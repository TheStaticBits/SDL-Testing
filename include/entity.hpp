#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "vector.hpp"
#include "window.hpp"

class Entity
{
public:
    Entity(SDL_Texture* tex, Vect<float> pos, bool initFrame=true);
    void destroy();

    Entity(const Entity& other) { pos = other.pos; texture = other.texture; frame = other.frame; }
    Entity& operator=(const Entity& other) { pos = other.pos; texture = other.texture; frame = other.frame; return *this; }

    void render(Window& window, const Vect<int>& offset = {0, 0});

    // Getter functions
    inline SDL_Texture* getImg() const { return texture; }

    inline SDL_Rect& getFrame() { return frame; }
    inline SDL_Rect getFrameCopy() const { return frame; }
    inline SDL_Rect getRect()
    {
        SDL_Rect result = frame;
        result.x = pos.x;
        result.y = pos.y;

        return result;
    }
    
    inline int getWidth() const { return frame.w; }
    inline int getHeight() const { return frame.h; }

    inline Vect<float>& getPos() { return pos; }
    inline Vect<float> getPosCopy() const { return pos; }
    inline int getX() const { return pos.x; }
    inline int getY() const { return pos.y; }

protected:
    Vect<float> pos;
    SDL_Texture* texture;
    SDL_Rect frame;
};