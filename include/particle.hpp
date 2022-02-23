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
    Particle(SDL_Texture* tex, const Vect<float>& pos, const int minSize, const int maxSize, const int speed, const int angle);

    bool update();
    void render(Window& window, const Vect<int>& offset = {0, 0});

private:
    const float dwindle = 0.2f;

    const int speed;
    const int angle;

    int size;
}