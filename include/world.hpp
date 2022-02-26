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
    World(Window& window);
    void destroy();

    void render(Window& window, const Vect<int>& offset = {0, 0});
    void update(const int yOffset);

    void genLayers(const int amount);
    void genLayer();

    void removePlatform(const int index);

    inline std::vector<Entity>& getPlatforms() { return platforms; }

private:
    static constexpr int layerStartY = 100;

    SDL_Texture* platformTex;
    std::vector<Entity> platforms;

    const Vect<int> platformSize;

    std::vector<Particle> particles;

    int layer = 0;
};