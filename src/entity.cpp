#include "entity.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <cmath>

#include "utility.hpp"
#include "vector.hpp"
#include "window.hpp"

Entity::Entity(SDL_Texture* tex, Vect<float> pos, bool initFrame)
    : pos(pos), texture(tex)
{
    if (initFrame)
    {
        frame.x = 0;
        frame.y = 0;

        const Vect<int> size = util::getImgSize(texture);
        frame.w = size.x;
        frame.h = size.y;
    }
}

void Entity::destroy() // So it isn't called in places where I don't need it to
{
    SDL_DestroyTexture(texture);
}

void Entity::render(Window& window, const Vect<int>& offset)
{
    SDL_Rect dest = { (int)std::floor(pos.x) + offset.x, (int)std::floor(pos.y) + offset.y, frame.w, frame.h };
    window.render(texture, dest);
}