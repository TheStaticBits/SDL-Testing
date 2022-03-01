#include "animation.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <math.h>

#include "utility.hpp"
#include "constants.hpp"
#include "window.hpp"

Animation::Animation(SDL_Texture* texture, const int width, const int frames, const int cooldown)
    : texture(texture), width(width), height(util::getImgSize(texture).y), frames(frames), cooldown(cooldown), currentFrame(0), cooldownTimer(0)
{}

void Animation::update()
{
    cooldownTimer++;
    
    if (cooldownTimer >= cooldown)
    {
        cooldownTimer = 0;
        currentFrame++;

        if (currentFrame >= frames)
            currentFrame = 0;
    }
}

void Animation::render(Window& window, const Vect<float>& renderPos)
{
    SDL_Rect src = { currentFrame * width, 0, width, height };
    SDL_Rect dest = { (int)round(renderPos.x), (int)round(renderPos.y), width, height };

    window.render(texture, src, dest);
}