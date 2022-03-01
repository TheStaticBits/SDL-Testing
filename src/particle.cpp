#include "particle.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <math.h>

#include "window.hpp"
#include "entity.hpp"
#include "vector.hpp"
#include "utility.hpp"
#include "constants.hpp"

Particle::Particle(SDL_Texture* tex, Vect<float>& pos, const float dwindle, const int minSize, const int maxSize, const float speed, const int angle)
    : Entity(tex, pos, false), dwindle(dwindle), speed(speed), angle(angle * (PI / 180))
{
    // Gets a random square of the texture

    // Random side length
    const int sideLength = rand() % (maxSize - minSize) + minSize;

    frame.w = sideLength;
    frame.h = sideLength;

    // Used for float percision
    size = sideLength;

    const Vect<int> texSize = util::getImgSize(tex);
    
    // Random position in the texture
    frame.x = std::rand() % (texSize.x - sideLength);
    frame.y = std::rand() % (texSize.y - sideLength);
}

// Copy constructor
Particle::Particle(const Particle& other)
    : Entity(other) 
{
    dwindle = other.dwindle;
    speed = other.speed;
    angle = other.angle;
    size = other.size;
}

// Copy assignment operator
Particle& Particle::operator=(const Particle& other) 
{
    Entity::operator=(other);
    dwindle = other.dwindle;
    speed = other.speed;
    angle = other.angle;
    size = other.size;
    return *this;
}

void Particle::update()
{
    // Moving in the direction of the angle
    pos.x += cos(angle) * speed;
    pos.y += sin(angle) * speed;

    size -= dwindle;

    frame.w = (int)round(size);
    frame.h = (int)round(size);
}

void Particle::render(Window& window, const Vect<int>& offset)
{
    SDL_Rect dest = { (int)round(pos.x) + offset.x - (int)round(size / 2), (int)round(pos.y) + offset.y - (int)round(size / 2), frame.w, frame.h };
    window.render(texture, frame, dest);
}