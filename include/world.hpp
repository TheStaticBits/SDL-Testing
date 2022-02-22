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

    void render(Window& window, const Vect<int>& offset = {0, 0});
    void update(const int yOffset);

    void genLayers(const int amount);
    void genLayer();

    inline std::vector<Entity>& getPlatforms() { return platforms; }
    inline void removePlatform(const int index) { platforms.erase(platforms.begin() + index); }

private:
    const int layerStartY = 100;
    const Vect<int> platformSize;

    SDL_Texture* platformTex;
    std::vector<Entity> platforms;

    int layer = 0;
};