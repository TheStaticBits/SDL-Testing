#include "window.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>

#include "constants.hpp"
#include "vector.hpp"
#include "utility.hpp"
#include "entity.hpp"

Window::Window(const char* title)
    : window(NULL), renderer(NULL), windowFrame{0, 0, AWINDOW_WIDTH, AWINDOW_HEIGHT}, miniWindowFrame{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}
{
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, AWINDOW_WIDTH, AWINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL)
        error("initialize window");
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

    miniWindow = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

    SDL_SetTextureBlendMode(miniWindow, SDL_BLENDMODE_BLEND);
}

void Window::destroy()
{
    //SDL_DestroyTexture(miniWindow);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Window::display()
{
    SDL_SetRenderTarget(renderer, NULL); // Set to window
    
    render(miniWindow, windowFrame); // Scaling up and rendering
    SDL_RenderPresent(renderer);
    
    SDL_SetRenderTarget(renderer, miniWindow); // Set to mini window
}

void Window::clear()
{
    // Clear window
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderClear(renderer);
    
    // Clear miniWindow
    SDL_SetRenderTarget(renderer, miniWindow);
    SDL_RenderClear(renderer);
}

SDL_Texture* Window::loadImage(const char* path)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, path);

    if (texture == NULL)
        error("load texture at " + std::string(path));
    
    return texture;
}

void Window::render(SDL_Texture* image, SDL_Rect& dest)
{
    if (SDL_RenderCopy(renderer, image, NULL, &dest) != 0)
        error("render texture");
}

void Window::render(SDL_Texture* image, SDL_Rect& src, SDL_Rect& dest)
{
    if (SDL_RenderCopy(renderer, image, &src, &dest) != 0)
        error("render texture with source");
}

void Window::drawRect(SDL_Rect& rect)
{
    if (SDL_RenderDrawRect(renderer, &rect) != 0)
        error("draw rect");
}

void Window::drawTexOnTex(SDL_Texture* image, SDL_Texture* drawOn, const Vect<float>& renderPos)
{
    SDL_SetTextureBlendMode(drawOn, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, drawOn); // Render target
    
    Vect<int> size = util::getImgSize(image);

    SDL_Rect dest;
    dest.x = renderPos.x;
    dest.y = renderPos.y;
    dest.w = size.x;
    dest.h = size.y;

    render(image, dest);

    SDL_SetRenderTarget(renderer, miniWindow); // Resetting
}