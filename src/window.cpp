#include "window.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>

#include "vector.hpp"
#include "utility.hpp"
#include "entity.hpp"

Window::Window(const char* title, const int width, const int height)
    : window(NULL), renderer(NULL)
{
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

    if (window == NULL)
        error("initialize window");
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Window::display()
{
    SDL_RenderPresent(renderer);
}

void Window::clear()
{
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

    SDL_SetRenderTarget(renderer, NULL); // Resetting
}

void Window::error(std::string action)
{
    std::cout << "Failed to " << action << ". Error: " << SDL_GetError() << std::endl;
}