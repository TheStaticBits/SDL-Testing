#include "game.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

#include "player.hpp"
#include "constants.hpp"

Game::Game()
    : window("HELLO", WINDOW_WIDTH, WINDOW_HEIGHT), player(window), world(window), offset{0, 0}
{
    // Sets the allowed keys to false
    for (SDL_Keycode key : allowedKeys)
        keys[key] = false;

    // Set seed
    std::srand(std::time(0));
}

Game::~Game()
{
    player.destroy();
    window.destroy();
    world.destroy();

    platforms.clear();

    IMG_Quit();
    SDL_Quit();
}

bool Game::initSDL() const
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || !IMG_Init(IMG_INIT_PNG))
    {
        std::cout << "SDL failed to initialize. Error: " << SDL_GetError() << std::endl;
        
        return false;
    }

    return true;
}

void Game::run()
{
    // Main loop
    while (!exit)
    {
        getInputs();
        
        update();
        render();

        window.display();
        window.clear();
    }
}

void Game::handleKey(SDL_Keycode& key, Uint32& type)
{
    if (key == SDLK_ESCAPE)
        exit = true;
    else if (std::find(allowedKeys.begin(), allowedKeys.end(), key) != allowedKeys.end())
        keys[key] = (type == SDL_KEYDOWN);
}

void Game::getInputs()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            exit = true;
        else
            handleKey(event.key.keysym.sym, event.type);
    }
}

void Game::update()
{
    player.update(keys, world);
    world.update(offset.y);
}

void Game::render()
{
    offset.y = WINDOW_HEIGHT / 2 - player.getY();
    if (offset.y > 0) offset.y = 0;
    
    player.render(window, offset);
    world.render(window, offset);
}