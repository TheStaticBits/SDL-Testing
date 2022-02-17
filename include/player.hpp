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

class Player : public Entity
{
public:
    Player(Window& window) : Entity(window.loadImage(PLAYER_PATH), 0, 0) {}

    void update(std::unordered_map<SDL_Keycode, bool>& keys, std::vector<Entity>& platforms);

private:
    const float gravity = 0.2f;
    const float speed = 5.0f;

    Vect<float> velocity;

    Vect<int> moveCheck(std::vector<Entity>& platforms);
    int checkAll(std::vector<Entity>& platforms);
};