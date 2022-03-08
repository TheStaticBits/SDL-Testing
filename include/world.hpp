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
enum PlatformType { Default, OnlyDash, Undashable };

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

    inline std::vector<std::pair<Entity, PlatformType>>& getPlatforms() { return platforms; }
    inline std::pair<Entity, PlatformType>& getPlatformAt(int index) { return platforms[index]; }
    
    inline void addEnergy(const float num) { energy += num; }
    inline void removeEnergy(const float num) { energy -= num; }

private:
    static constexpr unsigned int layerStartY = 100;
    static constexpr float partSpeedMult = 2.1f;
    const std::unordered_map<PlatformType, int> platChances = 
        {{Default,    70}, 
         {OnlyDash,   15}, 
         {Undashable, 15}};
    const std::unordered_map<PlatformType, std::string> platPaths = 
        {{Default,    "res/plats/norm.png" }, 
         {OnlyDash,   "res/plats/dash.png" }, 
         {Undashable, "res/plats/udash.png"}};

    const std::unordered_map<PlatformType, int> energyGain = 
        {{Default,    3}, 
         {OnlyDash,   6}, 
         {Undashable, 6}};

    std::unordered_map<PlatformType, SDL_Texture*> platTextures;
    std::vector<std::pair<Entity, PlatformType>> platforms;
    Vect<int> platformSize;

    std::vector<Particle> particles;

    SDL_Texture* energyBar;
    Vect<int> barSize;
    float energy;
    float displayEnergy;
    static constexpr float maxEnergy = 100.0f;
    static constexpr float energyDepletion = 0.1f;
    static constexpr float layerAddition = 0.005f; // Added depletion rate per layer created

    unsigned int layer = 0;

    unsigned int shakeTimer = 0; 
    Vect<int> shake = {0, 0};
};