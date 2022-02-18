#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <unordered_map>
#include <vector>

#include "window.hpp"
#include "world.hpp"
#include "player.hpp"

const std::vector<SDL_Keycode> allowedKeys = {
    SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN
};

class Game
{
public:
    Game();

    bool initSDL() const;

    void run();

    void quit();

private:
    void handleKey(SDL_Keycode& key, Uint32& type);
    void getInputs();

    void update();
    void render();

    std::unordered_map<SDL_Keycode, bool> keys;

    Window window;
    Player player;
    World world;
    std::vector<Entity> platforms;

    const int windowHeight = 600;
    const int windowWidth = 800;

    bool exit = false;
    SDL_Event event;
};