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
    genLayers(10);
}

void World::destroy()
{
    SDL_DestroyTexture(platformTex);

    platforms.clear();
    particles.clear();
}

void World::render(Window& window, const Vect<int>& offset)
{
    for (Entity& entity : platforms)
        entity.render(window, offset);

    for (Particle& particle : particles)
        particle.render(window, offset);
}

void World::update(const int yOffset)
{
    if (layerStartY + (layer * platformSize.y) + yOffset < WINDOW_HEIGHT)
        genLayer();

    for (auto i = particles.begin(); i != particles.end();)
    {
        i -> update();

        if (i -> dead()) 
            i = particles.erase(i);
        else 
            ++i;
    }

    for (Particle& particle : particles)
        particle.update();
}

void World::genLayers(const int amount)
{
    for (int i = 0; i < amount; i++)
        genLayer();
}

void World::genLayer()
{
    const int offset = (layer % 2 == 0 ? 0 : platformSize.x / 2);
    const int layersY = layerStartY + (layer++ * platformSize.y);

    for (int l = 0; l < WINDOW_WIDTH / platformSize.x + 1; l++)
        platforms.push_back(Entity(platformTex, { (float)(l * platformSize.x - offset), (float)layersY }));
}

void World::removePlatform(const int index)
{
    Vect<float> platCenter;
    platCenter.x = (float)(platforms[index].getX() + platformSize.x / 2);
    platCenter.y = (float)(platforms[index].getY() + platformSize.y / 2);

    // Creating particles
    for (int i = 0; i < 360; i += 5)
        particles.push_back(Particle(platformTex, platCenter, 15, 20, 1.2f, i));

    // Removing
    platforms.erase(platforms.begin() + index);
}