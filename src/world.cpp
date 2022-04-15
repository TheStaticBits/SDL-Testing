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
    : energy(100.0f), displayEnergy(100.0f), font(window, FONT_DATA_PATH, FONT_IMG_PATH)
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

    // int width = 40;
    // int height = 40;
    // SDL_Rect box = {WINDOW_WIDTH - width, 0, width, height};
    // window.drawRect(box, {0, 255, 0, 255});

    // font.render(window, {20, WINDOW_HEIGHT - 20}, "energy: " + std::to_string((int)round(displayEnergy)));
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
    if (shakeStrength > 1)
    {
        shake.x = (rand() % shakeStrength) - (int)floor(shakeStrength / 2);
        shake.y = (rand() % shakeStrength) - (int)floor(shakeStrength / 2);
        
        shakeStrength -= 2;
    } 
    else 
    {
        shake = {0, 0};
        shakeStrength = 0;
    }
    
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

void World::removePlatform(int index, float partSpeed, std::vector<Vect<float>> greenAgainPos)
{
    Vect<float> platformPos = platforms[index].first.getPos();

    // Getting the center of the platform for particles
    Vect<float> platCenter;
    platCenter.x = (float)(platforms[index].first.getX() + platformSize.x / 2);
    platCenter.y = (float)(platforms[index].first.getY() + platformSize.y / 2);
    
    partSpeed = std::sqrt(partSpeed) * partSpeedMult;
    Vect<int> partSize(10, 20); // Default particle size

    // Special bricks
    if (platforms[index].second == Exploder) // Green brick
    {
        partSize = explodeParticleSize; // Explosion particle size
        partSpeed = explodeParticleSpeed; // Explosion particle speed

        for (Vect<int>& box : explosionBoxes)
        {
            // Centers the collide box on the brick
            SDL_Rect collide = {(int)(platCenter.x - round(box.x / 2)), (int)(platCenter.y - round(box.y / 2)), box.x, box.y};

            std::vector<Vect<float>> remove;

            // Checking explosion with all platforms and removing when applicable
            // oh wow this could set off a chain reaction lol
            for (int i = 0, size = platforms.size(); i < size; i++)
                if (util::collide(platforms[i].first.getRect(), collide))
                    if (platforms[i].first.getPos() != platformPos)
                        remove.push_back(platforms[i].first.getPos());
            
            // Removing platforms
            for (int i = 0, size = remove.size(); i < size; i++)
            {
                const int platIndex = getPlatformIndex(remove[i]);

                if (platIndex != -1)
                {
                    // Preventing infinite recursion with more than one exploder blocks exploding each other

                    // Normal brick
                    if (platforms[platIndex].second != Exploder)
                        removePlatform(platIndex, explosionBrickPartSpeed);
                    // If the brick has not already been exploded
                    else if (std::find(greenAgainPos.begin(), greenAgainPos.end(), remove[i]) == greenAgainPos.end())
                    {
                        greenAgainPos.push_back(platformPos);
                        removePlatform(platIndex, partSpeed, greenAgainPos);
                    }  
                }
            }
        }

        // Normalizing screenshake
        shakeStrength /= 2;
    }

    // Getting the index after any explosions
    index = getPlatformIndex(platformPos);

    // Creating particles
    for (int i = 0; i < 360; i += 5)
        particles.push_back(Particle(platforms[index].first.getImg(), platCenter, 0.3f, partSize.x, partSize.y, partSpeed, i));

    // Adding energy for the platform
    energy += energyGain.at(platforms[index].second);
    // Shaking
    shakeStrength += screenShake.at(platforms[index].second);
    
    // Removing platform
    platforms.erase(platforms.begin() + index);
}

int World::getPlatformIndex(Vect<float> pos)
{
    // Returns negative -1 when not found
    for (int i = 0, size = platforms.size(); i < size; i++)
        if (pos == platforms[i].first.getPos())
            return i;

    return -1;
}