#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "utility.hpp"
#include "constants.hpp"
#include "window.hpp"
#include "vector.hpp"

class Animation 
{
public:
    Animation(SDL_Texture* texture, const int width, const int frames, const int cooldown);

    void update();
    void render(Window& window, const Vect<float>& renderPos);

private:
    SDL_Texture* texture;
    const int width;
    const int height;
    
    const int frames;
    const int cooldown;

    int currentFrame;
    int cooldownTimer;
};