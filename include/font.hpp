#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "window.hpp"
#include "vector.hpp"

class Font
{
public:
    Font(Window& window, const char* dataPath, const char* imgPath);
    ~Font();

    void render(Window& window, Vect<int> position, const std::string text, const int scale = 1);

private:
    nlohmann::json fontData;
    SDL_Texture* fontImg;
};