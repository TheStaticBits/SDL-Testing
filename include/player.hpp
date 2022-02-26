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

class Player : public Entity
{
public:
    Player(Window& window) : Entity(window.loadImage(PLAYER_PATH), {0, 0}) {}

    void update(std::unordered_map<SDL_Keycode, bool>& keys, World& world);

private:
    static constexpr float gravity = 0.2f;
    static constexpr float speed = 0.8f;
    static constexpr float maxSpeed = 3.0f;
    static constexpr float bounceVel = -5.0f;

    Vect<float> velocity;

    Vect<int> moveCheck(World& world);
    int checkAll(std::vector<Entity>& platforms);
};