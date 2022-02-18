#include "world.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>

#include "entity.hpp"
#include "window.hpp"

World::World(SDL_Texture* platformTex)
    : platformTex(platformTex)
{
    // Creating testing platforms
    platforms.push_back(Entity(platformTex, 0, 100));
    platforms.push_back(Entity(platformTex, 150, 150));
    platforms.push_back(Entity(platformTex, 300, 200));
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