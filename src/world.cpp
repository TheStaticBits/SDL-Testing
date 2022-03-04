#include "world.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>

#include "entity.hpp"
#include "window.hpp"
#include "constants.hpp"
#include "utility.hpp"
#include "particle.hpp"

World::World(Window& window)
    : platformTex(window.loadImage(PLATFORM_PATH)), platformSize(util::getImgSize(platformTex))
{

}

void World::destroy()
{
    SDL_DestroyTexture(platformTex);

    platforms.clear();
    particles.clear();
}

void World::render(Window& window, const Vect<int>& offset)
{
    for (std::vector<Entity, PlatformType>% plat : platforms)
        plat[0].render(window, offset);

    for (Particle& particle : particles)
        particle.render(window, offset);
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
        
        platforms.append({Entity(platformTex, { (float)(l * platformSize.x - offset), (float)layersY }), typeChosen});
    }
}

void World::removePlatform(const int index, const float partSpeed)
{
    Vect<float> platCenter;
    platCenter.x = (float)(platforms[index].getX() + platformSize.x / 2);
    platCenter.y = (float)(platforms[index].getY() + platformSize.y / 2);

    // Creating particles
    for (int i = 0; i < 360; i += 5)
        particles.push_back(Particle(platformTex, platCenter, 0.3f, 10, 20, speed, i));

    // Removing
    platforms.erase(platforms.begin() + index);

    // Shaking
    shakeTimer = 10;
}