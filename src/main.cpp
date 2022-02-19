#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "game.hpp"

int main(int argc, char* argv[])
{
    Game game;

    if (!game.initSDL())
    {
        SDL_Quit();
        return -1;
    }

    game.run();

    std::cout << "Press ENTER to close..." << std::endl;
    std::getchar();

    return 0;
}