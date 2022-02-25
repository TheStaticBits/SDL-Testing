#include "entity.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <cmath>

#include "utility.hpp"
#include "vector.hpp"
#include "window.hpp"

Entity::Entity(SDL_Texture* tex, Vect<float>& pos)
    : pos(pos), texture(tex)
{
    frame.x = 0;
    frame.y = 0;

    const Vect<int> size = util::getImgSize(texture);
    frame.w = size.x;
    frame.h = size.y;
}

Entity::~Entity()
{
    SDL_DestroyTexture(texture);
}

void Entity::render(Window& window, const Vect<int>& offset = {0, 0})
{
    SDL_Rect dest = { std::lround(pos.x) + offset.x, std::lround(pos.y) + offset.y, frame.w, frame.h };
    window.render(texture, dest);
}