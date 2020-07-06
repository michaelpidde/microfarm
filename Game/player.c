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
    _state.player.speed = 15;
    _state.player.velocity = 0;
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
    _state.player.velocity = direction_to_increment(direction);
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