#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <unordered_map>
#include <vector>

#include "constants.hpp"
#include "entity.hpp"
#include "vector.hpp"
#include "window.hpp"
#include "world.hpp"
#include "utility.hpp"

class Player : public Entity
{
public:
    Player(Window& window);

    void update(std::unordered_map<SDL_Keycode, bool>& keys, const SDL_Keycode doublePress, World& world);

private:
    static constexpr float gravity = 0.2f;
    static constexpr float speed = 0.8f;
    static constexpr float maxSpeed = 3.0f;
    static constexpr float bounceVel = -5.0f;
    
    static constexpr int dashDuration = 12;
    const Vect<float> dashSpeed = {8.0f, 5.0f};
    
    Vect<int> dash;
    int dashTimer;
    bool canDash;

    Vect<float> velocity;

    Vect<int> moveCheck(World& world);
    int checkAll(std::vector<std::pair<Entity, PlatformType>>& platforms);
};