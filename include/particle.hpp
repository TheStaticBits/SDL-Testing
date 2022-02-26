#pragma once

#include <SDL2/SDL.h>
#include <sDL2/SDL_image.h>

#include <iostream>

#include "window.hpp"
#include "entity.hpp"
#include "vector.hpp"

class Particle : public Entity
{
public:
    Particle(SDL_Texture* tex, Vect<float>& pos, const int minSize, const int maxSize, const float speed, const int angle);

    Particle(const Particle& other) : Entity(other) { speed = other.speed; angle = other.angle; size = other.size; }
    Particle& operator=(const Particle& other) { Entity::operator=(other); speed = other.speed; angle = other.angle; size = other.size; return *this; }

    bool update();
    void render(Window& window, const Vect<int>& offset = {0, 0});

private:
    static constexpr float dwindle = 0.2f;

    float speed;
    int angle;

    float size;
};