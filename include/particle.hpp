#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "window.hpp"
#include "entity.hpp"
#include "vector.hpp"

class Particle : public Entity
{
public:
    Particle(SDL_Texture* tex, Vect<float>& pos, const float dwindle, const int minSize, const int maxSize, const float speed, const int angle);

    Particle(const Particle& other);
    Particle& operator=(const Particle& other);

    void update();
    void render(Window& window, const Vect<int>& offset = {0, 0});

    inline bool dead() const { return size <= 0; }

private:
    float dwindle;

    float speed;
    float angle;

    float size;
};
