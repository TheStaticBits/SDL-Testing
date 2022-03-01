#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "game.hpp"

Game game;

void it()
{
    game.iteration();
}

int main(int argc, char* argv[])
{
    if (!game.initSDL())
    {
        SDL_Quit();
        return -1;
    }

    game.run();

    return 0;
}