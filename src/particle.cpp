#include "particle.hpp"

#include <SDL2/SDL.h>
#include <sDL2/SDL_image.h>

#include <iostream>
#include <cstdlib>
#include <math.h>

#include "window.hpp"
#include "entity.hpp"
#include "vector.hpp"
#include "utility.hpp"
#include "constants.hpp"

Particle::Particle(SDL_Texture* tex, const Vect<float>& pos, const int minSize, const int maxSize, const float speed, const int angle)
    : speed(speed), angle(angle * (PI / 180)), pos(pos), texture(tex)
{
    // Gets a random square of the texture

    // Random side length
    const int sideLength = std::rand() % (maxSize - minSize) + minSize;

    frame.w = sideLength;
    frame.h = sideLength;

    // Used for float percision
    size = sideLength;

    const Vect<int> texSize = util::getImgSize(tex);
    
    // Random position in the texture
    frame.x = std::rand() % (texSize.x - sideLength);
    frame.y = std::rand() % (texSize.y - sideLength);
}

bool Particle::update()
{
    // Moving in the direction of the angle
    pos.x += cos(angle) * speed;
    pos.y += sin(angle) * speed;

    size -= dwindle;

    frame.w = std::lround(size);
    frame.h = std::lround(size);

    // Returns whether to delete the particle from where it's stored
    return (size <= 0)
}

void Particle::render(Window& window, const Vect<int>& offset = {0, 0})
{
    SDL_Rect dest = { std::lround(pos.x) + offset.x - (size / 2), std::lround(pos.y) + offset.y - (size / 2), frame.w, frame.h };
    window.render(tex, frame, dest);
}