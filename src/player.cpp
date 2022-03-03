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

Player::Player(Window& window)
    : Entity(window.loadImage(PLAYER_PATH), {0, 0}), dash{0, 0}, dashTimer(0)
{

}

void Player::update(std::unordered_map<SDL_Keycode, bool>& keys, SDL_Keycode doublePress, World& world)
{
    // Applying movement
    if (keys[SDLK_LEFT])
        velocity.x -= speed;
    else if (keys[SDLK_RIGHT])
        velocity.x += speed;
    else
    {
        // Slowing down
        if (velocity.x > 0)
            velocity.x -= speed;
        else if (velocity.x < 0)
            velocity.x += speed;
        
        // Rounding down/up to zero
        if ((0 < velocity.x && velocity.x < speed) || (0 > velocity.x && velocity.x > speed))
            velocity.x = 0;
    }

    velocity.x = util::lock(velocity.x, -maxSpeed, maxSpeed);
    velocity.y += gravity; // Applying gravity

    // Checking dash
    if (doublePress == SDLK_RIGHT || doublePress == SDLK_LEFT || doublePress == SDLK_UP)
    {
        if (dashCooldown == 0)
        {
            // Dashed
            if (doublePress == SDLK_RIGHT) dash.x = 1;
            else if (doublePress == SDLK_LEFT) dash.x = -1;
            else dash.y = -1;
            
            dashTimer = dashDuration;
            dashCooldown = dashCooldownDur;
        }
    }

    // Updating dash timer
    if (dashTimer > 0)
    {
        if (--dashTimer <= 0)
            dash = {0, 0}; // Dash completed
        else
        {
            velocity = {dash.x * dashSpeed.x, dash.y * dashSpeed.y}; // Dashing 
        }
    }

    if (dashCooldown > 0) --dashCooldown;

    // Applying velocity
    Vect<int> collisions = moveCheck(world);

    // Collided with a platform below
    if (collisions.y == 1)
        velocity.y = bounceVel; // bounce

    else if (collisions.y == -1)
    {
        // Collided with a platform above while not dashing
        if (dash.y == 0)
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
            if (dash.x == 0)
            {
                if (velocity.x < 0) // Left
                {
                    pos.x = world.getPlatforms()[collided].getPos().x + world.getPlatforms()[collided].getFrame().w;
                    collisions.x = -1;
                }
                else if (velocity.x > 0) // Right
                {
                    pos.x = world.getPlatforms()[collided].getPos().x - frame.w;
                    collisions.x = 1;
                }
            }
            else 
                world.removePlatform(collided); // Dashing
        }

        // Testing if the player is off the screen on either side
        if (pos.x < 0) 
        {
            pos.x = 0;
            dash.x = 0;
        }
        else if (pos.x > WINDOW_WIDTH - frame.w)
        { 
            pos.x = WINDOW_WIDTH - frame.w;
            dash.x = 0;
        }
    }

    // Moving and checking Y position
    if (velocity.y != 0)
    {
        pos.y += velocity.y;

        collided = checkAll(world.getPlatforms());

        if (collided != -1)
        {
            if (dash.y == 0)
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
            else
                world.removePlatform(collided); // Dashed up
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