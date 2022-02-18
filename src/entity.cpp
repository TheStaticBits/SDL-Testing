#include "entity.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <cmath>

#include "utility.hpp"
#include "vector.hpp"
#include "window.hpp"

Entity::Entity(SDL_Texture* tex, float x, float y)
    : pos(x, y), texture(tex)
{
    frame.x = 0;
    frame.y = 0;

    Vect<int> size = util::getImgSize(texture);
    frame.w = size.x;
    frame.h = size.y;
}

void Entity::destroy()
{
    SDL_DestroyTexture(texture);
}

void Entity::render(Window& window)
{
    SDL_Rect dest = { (int)std::round(pos.x), (int)std::round(pos.y), frame.w, frame.h };
    window.render(texture, dest);
}