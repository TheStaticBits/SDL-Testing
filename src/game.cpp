#include "game.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/html5.h>
#endif

#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

#include "player.hpp"
#include "constants.hpp"

Game::Game()
    : window("HELLO"), player(window), world(window), offset{0, 0}, waitUntil(0)
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

void it(); // Defined in main.cpp, calls Game::iteration()

void Game::run()
{
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(it, 0, 1);
#else
    // Main loop
    while (!exit)
        iteration();
#endif
}

void Game::iteration()
{
    getInputs();
        
    update();
    render();

    window.display(world.getShake());
    window.clear();

    cap();
}

void Game::cap()
{
    // Capping FPS
    if (capFPS && SDL_GetTicks64() < waitUntil)
        SDL_Delay(waitUntil - SDL_GetTicks64());
    
    // Updating waitUntil
    waitUntil = SDL_GetTicks64() + (1000 / FPS);
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