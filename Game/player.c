#include "microfarm.h"

/**
 * Set up default player data
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 */
void init_player()
{
    _state.player.asset_key = "actor_player";
    _state.player.position.x = 100;
    _state.player.position.y = 100;
    _state.player.speed = 7;
    _state.player.velocity = 0;
}


/**
 * Determines if player is going to collide with a world object in next
 * movement in given direction.
 * 
 * INPUT:
 * Direction -- Direction of player movement
 * int       -- Amount of pixels to try to move to
 * 
 * OUTPUT:
 * Rect *    -- World object collided with
 */
Rect *check_collision(Direction direction, int increment)
{
    // TODO: Remove hard coded size
    Rect player = {
        .x = _state.player.position.x,
        .y = _state.player.position.y,
        .w = PLAYER_W,
        .h = PLAYER_H
    };
    if(direction == North || direction == South) {
        player.y += increment;
    } else {
        player.x += increment;
    }

    for(int i = 0; i < _state.collision_rect_ctr; ++i) {
        Rect *collision = &_state.collision_rects[i];
        if(MCR_rect_overlap(&player, collision)) {
            return collision;
        }
    }

    return NULL;
}


/**
 * Increment player position on 2D grid
 * 
 * INPUT:
 * Direction -- The direction to move player
 * 
 * OUTPUT: none
 */
void player_move(Direction direction)
{
    int increment = direction_to_increment(direction);
    Rect *collided_with = check_collision(direction, increment * _state.player.speed);
    if(!collided_with) {
        _state.player.velocity = increment;
        switch(direction) {
            case North:
            case South: {
                _state.player.position.y += _state.player.velocity * _state.player.speed;
            } break;
            case East:
            case West: {
                _state.player.position.x += _state.player.velocity * _state.player.speed;
            } break;
        }
    } else {
        // Move player to be flush against collision object with 1 pixel buffer so movement in other directions
        // does not get stuck.
        switch(direction) {
            case North: {
                _state.player.position.y = collided_with->y + collided_with->h + 1;
            } break;
            case South: {
                _state.player.position.y = collided_with->y - PLAYER_H - 1;
            } break;
            case East: {
                _state.player.position.x = collided_with->x - PLAYER_W - 1;
            } break;
            case West: {
                _state.player.position.x = collided_with->x + collided_with->w + 1;
            } break;
        }
    }
}