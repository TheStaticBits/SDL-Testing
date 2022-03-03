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
    ~Game();

    bool initSDL() const;

    void run();
    void iteration();

private:
    static constexpr int FPS = 60;
    static constexpr int keyCountdown = 20;

    void handleKey(SDL_Keycode& key, Uint32& type);
    void getInputs();

    void update();
    void render();

    std::unordered_map<SDL_Keycode, bool> keys;

    std::unordered_map<SDL_Keycode, bool> doublePress;
    std::unordered_map<SDL_Keycode, int> dPressCountdown;

    Window window;
    Player player;
    World world;
    std::vector<Entity> platforms;

    Vect<int> offset;

    bool exit = false;
    SDL_Event event;
};