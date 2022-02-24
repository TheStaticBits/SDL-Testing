#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>

#include "entity.hpp"
#include "window.hpp"
#include "particle.hpp"

class World 
{
public:
    World(SDL_Texture* platformTex);
    void destroy();

    void render(Window& window, const Vect<int>& offset = {0, 0});
    void update(const int yOffset);

    void genLayers(const int amount);
    void genLayer();

    void removePlatform(const int index);

    inline std::vector<Entity>& getPlatforms() { return platforms; }

private:
    const int layerStartY = 100;
    const Vect<int> platformSize;

    SDL_Texture* platformTex;
    std::vector<Entity> platforms;

    std::vector<Particle> particles;

    int layer = 0;
};