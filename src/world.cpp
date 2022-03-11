#include "world.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "entity.hpp"
#include "window.hpp"
#include "constants.hpp"
#include "utility.hpp"
#include "particle.hpp"

World::World(Window& window)
    : energy(100.0f), displayEnergy(100.0f)
{
    // Load platform textures
    for (const auto& platformPair : platPaths)
        platTextures[platformPair.first] = window.loadImage(platformPair.second.c_str());
    
    platformSize = util::getImgSize(platTextures[Default]);

    energyBar = window.loadImage(BAR_PATH);
    barSize = util::getImgSize(energyBar);

    // Initializing exploder boxes
    for (const Vect<float>& percents : exploderCollide)
        explosionBoxes.push_back({(int)round(percents.x * platformSize.x), (int)round(percents.y * platformSize.y)});
    // ^ I really need to add a constructor so I can easily multiply vectors with different types
}

void World::destroy()
{
    for (const auto& tex : platTextures)
        SDL_DestroyTexture(tex.second);

    platforms.clear();
    particles.clear();
}

void World::render(Window& window, const Vect<int>& offset)
{
    for (std::pair<Entity, PlatformType>& plat : platforms)
        plat.first.render(window, offset);

    for (Particle& particle : particles)
        particle.render(window, offset);

    SDL_Rect barRect = {BAR_X, BAR_Y, (int)round(displayEnergy * (barSize.x / maxEnergy)), barSize.y};
    window.drawRect(barRect, BAR_COLOR);

    barRect.w = barSize.x;
    window.render(energyBar, barRect);
}

void World::update(const int yOffset)
{
    // Generating layers if needed
    if (layerStartY + (layer * platformSize.y) + yOffset < WINDOW_HEIGHT)
        genLayer();

    // Updating particles
    for (auto i = particles.begin(); i != particles.end();)
    {
        i -> update();

        if (i -> dead()) 
            i = particles.erase(i);
        else 
            ++i;
    }

    // Updating shake timer
    if (shakeTimer > 0)
    {
        shakeTimer--;

        if (shakeTimer % 2 == 0)
        {
            shake.x = (rand() % 9) - 4;
            shake.y = (rand() % 9) - 4;
        }
    } 
    else 
        shake = {0, 0};
    
    // Updating energy bar
    energy -= energyDepletion + (layer * layerAddition);

    energy = util::lock(energy, 0.0f, maxEnergy);

    // Moving display energy towards actual energy
    if (energy > displayEnergy)
        displayEnergy += (energy - displayEnergy) / 20;
    else if (energy < displayEnergy)
        displayEnergy -= (displayEnergy - energy) / 20;
}

void World::genLayers(const int amount)
{
    for (int i = 0; i < amount; i++)
        genLayer();
}

void World::genLayer()
{
    const int offset = (layer % 2 == 0 ? 0 : platformSize.x / 2);
    const int additional = (layer % 2 == 0 ? 0 : 1);
    const int layersY = layerStartY + (layer++ * platformSize.y);

    for (int l = 0, limit = WINDOW_WIDTH / platformSize.x + additional; l < limit; l++)
    {
        PlatformType typeChosen;
        int random = rand() % 100;

        // Iterates through and chooses the platform from the random number
        for (const auto& platType : platChances)
        {
            if (random < platType.second)
            {
                typeChosen = platType.first;
                break;
            }
            else
                random -= platType.second;
        }
        
        platforms.push_back({Entity(platTextures[typeChosen], { (float)(l * platformSize.x - offset), (float)layersY }), typeChosen});
    }
}

void World::removePlatform(const int index, float partSpeed, const bool explode2nd)
{
    partSpeed = std::sqrt(partSpeed) * partSpeedMult;

    Vect<float> platCenter;
    platCenter.x = (float)(platforms[index].first.getX() + platformSize.x / 2);
    platCenter.y = (float)(platforms[index].first.getY() + platformSize.y / 2);

    Vect<float> platformPos = platforms[index].first.getPos();

    // Creating particles
    for (int i = 0; i < 360; i += 5)
        particles.push_back(Particle(platforms[index].first.getImg(), platCenter, 0.3f, 10, 20, partSpeed, i));

    // Adding energy for the platform
    energy += energyGain.at(platforms[index].second);
    // Shaking
    shakeTimer = 10;

    int totalRemoved = 0;

    // Special bricks
    if (platforms[index].second == Exploder)
    {
        for (Vect<int>& box : explosionBoxes)
        {
            // Centers the collide box on the brick
            SDL_Rect collide = {(int)(platCenter.x - round(box.x / 2)), (int)(platCenter.y - round(box.y / 2)), box.x, box.y};

            std::vector<int> remove;
            bool beforePlatform = true;

            // Checking explosion with all platforms and removing when applicable
            // oh wow this could set off a chain reaction lol
            for (int i = 0, size = platforms.size(); i < size; i++)
                if (util::collide(platforms[i].first.getRect(), collide))
                {
                    if (platforms[i].first.getPos() == platformPos)
                        beforePlatform = false;
                    else
                    {
                        remove.push_back(i);

                        if (beforePlatform)
                            totalRemoved += 1;
                    }
                }
            
            // Removing platforms
            for (int i = 0, size = remove.size(); i < size; i++)
            {
                const bool secondExplode = platforms[remove[i]].second == Exploder;
                
                if (secondExplode)
                    std::cout << "boi" << std::endl;

                // Preventing infinite recursion with two exploder blocks
                if (!secondExplode || !explode2nd)
                    removePlatform(remove[i] - i, explosionPartSpeed, secondExplode);
            }
        }
    }
    
    platforms.erase(platforms.begin() + index - totalRemoved);
}