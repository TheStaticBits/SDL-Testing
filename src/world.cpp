#include "world.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>

#include "entity.hpp"
#include "window.hpp"
#include "constants.hpp"
#include "utility.hpp"

World::World(SDL_Texture* platformTex)
    : platformSize(util::getImgSize(platformTex)), platformTex(platformTex)
{
    genLayers(10);
}

void World::destroy()
{
    SDL_DestroyTexture(platformTex);

    for (Entity& entity : platforms)
        entity.destroy();
}

void World::render(Window& window)
{
    for (Entity& entity : platforms)
        entity.render(window);
}

void World::genLayers(const int amount)
{
    for (int i = 0; i < amount; i++)
    {
        const int offset = (layer % 2 == 0 ? 0 : platformSize.x / 2);
        const int layersY = layerStartY + (layer++ * platformSize.y);

        for (int l = 0; l < WINDOW_WIDTH / platformSize.x + 1; l++)
            platforms.push_back(Entity(platformTex, l * platformSize.x - offset, layersY));
    }
}