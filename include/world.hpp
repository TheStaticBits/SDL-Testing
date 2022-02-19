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
    inline void removePlatform(const int index) { platforms.erase(platforms.begin() + index); }

private:
    SDL_Texture* platformTex;

    std::vector<Entity> platforms;
};