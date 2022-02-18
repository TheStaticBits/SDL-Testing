#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>

#include "entity.hpp"
#include "window.hpp"

class World 
{
public:
    World(SDL_Texture* platformTex);
    
    void destroy();

    void render(Window& window);

    inline std::vector<Entity>& getPlatforms() { return platforms; }

private:
    SDL_Texture* platformTex;

    std::vector<Entity> platforms;
};