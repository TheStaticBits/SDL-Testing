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
#include "font.hpp"

// Possible types of platforms
enum PlatformType { Default, OnlyDash, Undashable, Exploder};

class World 
{
public:
    World(Window& window);
    void destroy();

    void render(Window& window, const Vect<int>& offset = {0, 0});
    void update(const int yOffset);

    void genLayers(const int amount);
    void genLayer();

    void removePlatform(int index, const float partSpeed, std::vector<Vect<float>> greenAgainPos={});

    inline Vect<int> getShake() const { return shake; }

    inline std::vector<std::pair<Entity, PlatformType>>& getPlatforms() { return platforms; }
    inline std::pair<Entity, PlatformType>& getPlatformAt(int index) { return platforms[index]; }
    
    inline void addEnergy(const float num) { energy += num; }
    inline void removeEnergy(const float num) { energy -= num; }

private:
    static constexpr unsigned int layerStartY = 100;
    static constexpr float partSpeedMult = 2.7f;

    // Multiplies by the size of a platform at runtime and centers on the exploder brick
    // This is only used at startup, see default constructor
    const std::vector<Vect<float>> exploderCollide = {{0.2, 4.5}, {2.5, 0.2}};
    std::vector<Vect<int>> explosionBoxes;
    static constexpr float explosionBrickPartSpeed = 4.0f;
    static constexpr float explodeParticleSpeed = 7.0f;
    const Vect<int> explodeParticleSize = {15, 25};
    // Platform constants
    const std::unordered_map<PlatformType, int> platChances = 
        {{Default,    65},  // White
         {OnlyDash,   20},  // Red 
         {Undashable, 10},  // Blue
         {Exploder,   5 }}; // Green
    const std::unordered_map<PlatformType, std::string> platPaths = 
        {{Default,    "res/plats/norm.png" }, 
         {OnlyDash,   "res/plats/dash.png" }, 
         {Undashable, "res/plats/udash.png"},
         {Exploder,   "res/plats/explode.png"}};
    const std::unordered_map<PlatformType, int> energyGain = 
        {{Default,    3}, 
         {OnlyDash,   6}, 
         {Undashable, 6},
         {Exploder,  -3}};
    const std::unordered_map<PlatformType, int> screenShake = 
        {{Default,    17}, 
         {OnlyDash,   27}, 
         {Undashable, 27},
         {Exploder,   15}};

    std::unordered_map<PlatformType, SDL_Texture*> platTextures;
    std::vector<std::pair<Entity, PlatformType>> platforms;
    Vect<int> platformSize;

    std::vector<Particle> particles;

    SDL_Texture* energyBar;
    Vect<int> barSize;
    float energy;
    float displayEnergy;
    static constexpr float maxEnergy = 100.0f;
    static constexpr float energyDepletion = 0.05f;
    static constexpr float layerAddition = 0.001f; // Added depletion rate per layer created

    unsigned int layer = 0;

    unsigned int shakeStrength = 0; 
    Vect<int> shake = {0, 0};

    int getPlatformIndex(Vect<float> pos);

    Font font;
};