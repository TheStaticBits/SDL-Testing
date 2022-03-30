#include "font.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <fstream>
#include <string>

#include <nlohmann/json.hpp>

#include "window.hpp"

Font::Font(Window& window, const char* dataPath, const char* imgPath)
    : fontData(nlohmann::json::parse(std::ifstream(dataPath))), fontImg(window.loadImage(imgPath))
{

}

Font::~Font()
{
    SDL_DestroyTexture(fontImg);
}

void Font::render(Window& window, Vect<int> position, const std::string text, const int scale)
{
    const Vect<int> distance(((int)fontData["dimensions"]["width"] + (int)fontData["dimensions"]["offset"]), ((int)fontData["dimensions"]["height"] + (int)fontData["dimensions"]["offset"]));

    for (const char& c : text)
    {
        if (c == '\n')
        {
            position.y += distance.y * scale;
            continue;
        }
        else if (c == ' ')
        {
            position.x += distance.x * scale;
            continue;
        }
        else
        {
            // Finding character position in fontData
            Vect<int> charPos(0, 0);
            bool found = false;
            for (const std::string& row : fontData["content"])
            {
                for (const char& charInRow : row)
                {
                    if (charInRow == c)
                    {
                        found = true;
                        break;
                    }
                    else charPos.x += distance.x;
                }
                
                if (found) break;
                else 
                {
                    charPos.x = 0;
                    charPos.y += distance.y;
                }
            }

            if (!found) continue;

            // Rendering character
            SDL_Rect srcRect = {charPos.x, charPos.y, (int)fontData["dimensions"]["width"], (int)fontData["dimensions"]["height"]};
            SDL_Rect dstRect = {position.x, position.y, (int)fontData["dimensions"]["width"] * scale, (int)fontData["dimensions"]["height"] * scale};

            window.render(fontImg, srcRect, dstRect);
        }
        
        position.x += distance.x * scale; // Render position
    }
}