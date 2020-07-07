#include "microfarm.h"

/*******************************************************************************
 * Set up default player data
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 ******************************************************************************/
void init_player()
{
    _state.player.asset_key = "actor_player";
    _state.player.position.x = 100;
    _state.player.position.y = 100;
    _state.player.speed = 5;
    _state.player.velocity = 0;
}


int collision(Direction direction, int increment)
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
        Rect collision = _state.collision_rects[i];
        if(MCR_rect_overlap(player, collision)) {
            return 1;
        }
    }
    
    return 0;
}


/*******************************************************************************
 * Increment player position on 2D grid
 * 
 * INPUT:
 * Direction -- The direction to move player
 * 
 * OUTPUT: none
 ******************************************************************************/
void player_move(Direction direction)
{
    int increment = direction_to_increment(direction);
    if(!collision(direction, increment * _state.player.speed)) {
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
    }
}