#include "player.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <unordered_map>
#include <vector>

#include "window.hpp"
#include "utility.hpp"
#include "constants.hpp"
#include "vector.hpp"

void Player::update(std::unordered_map<SDL_Keycode, bool>& keys, World& world)
{
    // Applying movement
    velocity.x = (keys[SDLK_RIGHT] - keys[SDLK_LEFT]);
    // Applying gravity
    velocity.y += gravity;

    // Applying velocity
    Vect<int> collisions = moveCheck(world);
    
    if (pos.y >= WINDOW_HEIGHT - frame.h)
        pos.y = WINDOW_HEIGHT - frame.h;

    // Collided with a platform below
    if (collisions.y == 1)
    {
        // // Applying jump or resetting velocity
        // if (keys[SDLK_UP]) velocity.y = -5;
        // else velocity.y = 0;

        velocity.y = -5; // bounce
    }
    // Collided with a platform above
    else if (collisions.y == -1)
    {
        velocity.y = 0;
    }
}

Vect<int> Player::moveCheck(World& world)
{
    Vect<int> collisions(0, 0);
    
    int collided = 0;

    // Moving and checking X position
    if (velocity.x != 0)
    {
        pos.x += velocity.x;

        collided = checkAll(world.getPlatforms());

        if (collided != -1)
        {
            if (velocity.x < 0) // Left
            {
                pos.x = world.getPlatforms()[collided].getPos().x + world.getPlatforms()[collided].getFrame().w;
                collisions.x = -1;
            }
            else // Right
            {
                pos.x = world.getPlatforms()[collided].getPos().x - frame.w;
                collisions.y = 1;
            }
        }
    }

    // Moving and checking Y position
    if (velocity.y != 0)
    {
        pos.y += velocity.y;

        std::cout << "before" << std::endl;
        collided = checkAll(world.getPlatforms());
        std::cout << "after" << std::endl;

        if (collided != -1)
        {
            if (velocity.y < 0) // Upward
            {
                pos.y = world.getPlatforms()[collided].getPos().y + world.getPlatforms()[collided].getFrame().h;
                collisions.y = -1;
            }
            else // Downward
            {
                pos.y = world.getPlatforms()[collided].getPos().y - frame.h;
                collisions.y = 1;
                world.removePlatform(collided);
            }
        }
    }

    return collisions;
}

int Player::checkAll(std::vector<Entity>& platforms)
{
    for (int i = 0, size = platforms.size(); i < size; i++)
        if (util::collide(*this, platforms[i]))
            return i;
    
    return -1;
}