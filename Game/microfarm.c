#include "microfarm.h"

State _state;

#include "unitybuild.c"


void update()
{
    // Add collision objects (world setup)
    // Collision object needs to be pushed before we try to do player movement.
    // TODO: Remove, just testing
    Rect rock_pos = {.x = 400, .y = 400, .w = 128, .h = 128};
    Rect rock_col = {
        .x = rock_pos.x + 10,
        .y = rock_pos.y + 10,
        .w = rock_pos.w - 20,
        .h = rock_pos.h - 60
    };
    MCR_push_collision_object(rock_col);
    Rect rock2_pos = {.x = 700, .y = 400, .w = 64, .h = 64};
    Rect rock2_col = {
        .x = rock2_pos.x + 10,
        .y = rock2_pos.y + 10,
        .w = rock2_pos.w - 20,
        .h = rock2_pos.h - 50
    };
    MCR_push_collision_object(rock2_col);

    
    if(_state.keys_down.w) {
        player_move(North);
    }
    if(_state.keys_down.a) {
        player_move(West);
    }
    if(_state.keys_down.s) {
        player_move(South);
    }
    if(_state.keys_down.d) {
        player_move(East);
    }

    /*
     * Set up sprite batch
     */

    // Add background
    int dimensions[2] = {0, 0};
    MCR_get_output_tiles(dimensions);

    Rect src = {.x = 0, .y = 0, .w = TILE_SIZE, .h = TILE_SIZE};
    Rect dest = {.x = 0, .y = 0, .w = TILE_SIZE, .h = TILE_SIZE};
    for(int row = 0; row < dimensions[1]; ++row) {
        for(int col = 0; col < dimensions[0]; ++col) {
            dest.x = col * TILE_SIZE;
            dest.y = row * TILE_SIZE;
            MCR_push_sprite("world_terrain", src, dest);
        }
    }

    // TODO: Remove, just testing
    // Add world collision ojects on top of background
    src.w = 128;
    src.h = 128;
    MCR_push_sprite("world_rock", src, rock_pos);
    src.w = TILE_SIZE;
    src.h = TILE_SIZE;
    MCR_push_sprite("world_rock2", src, rock2_pos);

    // Add player
    Rect player_src = {.x = 0, .y = 0, .w = PLAYER_W, .h = PLAYER_H};
    Rect rect = {.x = _state.player.position.x, .y = _state.player.position.y, .w = PLAYER_W, .h = PLAYER_H};
    MCR_push_sprite(_state.player.asset_key, player_src, rect);
}


void render()
{
}


void keyboard(char sym, int down)
{
    switch(sym) {
        case KEY_q: {
            MRC_quit();
        } break;
        case KEY_w: {
            if(down) {
                _state.keys_down.w = 1;
            } else {
                _state.keys_down.w = 0;
            }
        } break;
        case KEY_a: {
            if(down) {
                _state.keys_down.a = 1;
            } else {
                _state.keys_down.a = 0;
            }
        } break;
        case KEY_s: {
            if(down) {
                _state.keys_down.s = 1;
            } else {
                _state.keys_down.s = 0;
            }
        } break;
        case KEY_d: {
            if(down) {
                _state.keys_down.d = 1;
            } else {
                _state.keys_down.d = 0;
            }
        } break;
    }
}


void mouse(uint32 button, uint32 x, uint32 y, uint32 down)
{
}


int main()
{
    MCR_init("Micro Farm");
    MCR_set_tile_size(TILE_SIZE);

    MCR_load_asset_class("assets\\actor", "actor");
    MCR_load_asset_class("assets\\world", "world");
    MCR_register_palette("world_terrain");
    // loaded = MCR_load_asset_class("assets\\icons", "icon");
    // printf("Assets loaded from icon: %d\n", loaded);

    init_player();

    // TODO: I don't like passing an image path into this since we already loaded assets. But this needs an SDL_Surface
    MCR_set_cursor("assets\\icons\\cursor.png");

    MCR_run(&update, &render, &keyboard, &mouse);
}