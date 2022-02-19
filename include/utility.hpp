#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "vector.hpp"
#include "entity.hpp"

namespace util
{
    inline Vect<int> getImgSize(SDL_Texture* texture)
    {
        Vect<int> result(0, 0);
        SDL_QueryTexture(texture, NULL, NULL, &result.x, &result.y);

        return result;
    }

    inline bool collide(const SDL_Rect& rect1, const SDL_Rect& rect2)
    {
        return (rect1.x < rect2.x + rect2.w &&
                rect1.y < rect2.y + rect2.h &&
                rect2.x < rect1.x + rect1.w &&
                rect2.y < rect1.y + rect1.h);
    }

    inline bool collide(Entity& entity1, Entity& entity2)
    {
        SDL_Rect rect1 = entity1.getFrameCopy();
        rect1.x = entity1.getPos().x;
        rect1.y = entity1.getPos().y;

        SDL_Rect rect2 = entity2.getFrameCopy();
        rect2.x = entity2.getPos().x;
        rect2.y = entity2.getPos().y;

        return collide(rect1, rect2);
    }

    inline float lock(const float value, const float min, const float max)
    {
        if (value < min) return min;
        else if (value > max) return max;
        else return value;
    }
};