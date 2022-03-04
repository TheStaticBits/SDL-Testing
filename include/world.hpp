#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include "entity.hpp"
#include "window.hpp"
#include "particle.hpp"

// Possible types of platforms
enum PlatformType { Default, OnlyDash, Undashable }

class World 
{
public:
    World(Window& window);
    void destroy();

    void render(Window& window, const Vect<int>& offset = {0, 0});
    void update(const int yOffset);

    void genLayers(const int amount);
    void genLayer();

    void removePlatform(const int index, const float partSpeed);

    inline Vect<int> getShake() const { return shake; }

    inline std::vector<std::vector<Entity, PlatformType>>& getPlatforms() { return platforms; }
    inline std::vector<Entity, PlatformType>& getPlatformAt(int index) { return platforms[index]; }

private:
    static constexpr unsigned int layerStartY = 100;
    static const std::unordered_map<PlatformType, int> platChances = ({{Default,    70},
                                                                       {OnlyDash,   15},
                                                                       {Undashable, 15}});

    SDL_Texture* platformTex;
    std::vector<std::vector<Entity, PlatformType>> platforms;

    const Vect<int> platformSize;

    std::vector<Particle> particles;

    unsigned int layer = 0;

    unsigned int shakeTimer = 0; 
    Vect<int> shake = {0, 0};
};