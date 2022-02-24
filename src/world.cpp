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

World::World(SDL_Texture* platformTex)
    : platformSize(util::getImgSize(platformTex)), platformTex(platformTex)
{
    genLayers(10);
}

void World::destroy()
{
    SDL_DestroyTexture(platformTex);
}

void World::render(Window& window, const Vect<int>& offset = {0, 0})
{
    for (Entity& entity : platforms)
        entity.render(window, offset);
}

void World::update(const int yOffset)
{
    if (yOffset + WINDOW_HEIGHT > layerStartY + (layer * platformSize.y))
        genLayer();

    for (auto i = platforms.begin(), i != platforms.end();)
    {
        if (i -> update()) // If the particle should be removed
            i = platforms.erase(i);
        else
            ++i;
    }
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
        platforms.push_back(Entity(platformTex, l * platformSize.x - offset, layersY));
}

void World::removePlatform(const int index)
{
    const Vect<int> platCenter = { platforms[i].getX() + platformSize.x / 2, platforms.[i].getY() + platformSize.y / 2};

    // Creating particles
    for (int i = 0; i < 360; i += 30)
        particles.append(Particle(platformTex, platCenter, 10, 20, 1, i));

    // Removing
    platforms.erase(platforms.begin() + index);
}