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

    inline Vect<int> getShake() const { return shake; }

    inline std::vector<Entity>& getPlatforms() { return platforms; }

private:
    static constexpr unsigned int layerStartY = 100;

    SDL_Texture* platformTex;
    std::vector<Entity> platforms;

    const Vect<int> platformSize;

    std::vector<Particle> particles;

    unsigned int layer = 0;

    unsigned int shakeTimer = 0; 
    Vect<int> shake = {0, 0};
};