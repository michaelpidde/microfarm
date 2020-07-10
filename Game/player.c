#include "microfarm.h"


void init_player()
{
    _state.player.asset_key = "actor_guy";
    _state.player.position.x = 100;
    _state.player.position.y = 100;
    _state.player.speed = 7;
    _state.player.velocity = 0;
}


void player_move(Direction direction)
{
    int increment = direction_to_increment(direction);

    Rect next_position = {
        .x = _state.player.position.x,
        .y = _state.player.position.y,
        .w = PLAYER_W,
        .h = PLAYER_H
    };
    if(direction == North || direction == South) {
        next_position.y += increment * _state.player.speed;
    } else {
        next_position.x += increment * _state.player.speed;
    }

    Rect *collided_with = MCR_check_collision(next_position);
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