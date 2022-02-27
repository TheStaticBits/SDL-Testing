#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>

#include "vector.hpp"
#include "constants.hpp"

class Window
{
public:
    Window(const char* title);
    void destroy();

    void display(Vect<int> offset);
    void clear();

    SDL_Texture* loadImage(const char* path);
    void render(SDL_Texture* image, SDL_Rect& dest);
    void render(SDL_Texture* image, SDL_Rect& src, SDL_Rect& dest);
    void drawRect(SDL_Rect& action);

    void drawTexOnTex(SDL_Texture* image, SDL_Texture* drawOn, const Vect<float>& renderPos);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* miniWindow;

    static inline void error(std::string action)
    {
        std::cout << "Failed to " << action << ". Error: " << SDL_GetError() << std::endl;
    }
};