#include "microfarm.h"

State _state;

/**
 * Single compilation unit.
 */
#include "unitybuild.c"


/**
 * Primary entry point for game state updating. This gets passed into
 * MCR_run as a callback.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 */
void update()
{
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

    for(int row = 0; row < dimensions[1]; ++row) {
        for(int col = 0; col < dimensions[0]; ++col) {
            Rect r = {.x = col * TILE_SIZE, .y = row * TILE_SIZE, .w = TILE_SIZE, .h = TILE_SIZE};
            MCR_push_sprite("world_terrain", r);
        }
    }

    // Add collision object
    // TODO: Remove, just testing
    {
        // Reset collision counter or things will go wonky.
        _state.collision_rect_ctr = 0;

        Rect rock_pos = {.x = 400, .y = 400, .w = 128, .h = 128};
        MCR_push_sprite("world_rock", rock_pos);
        Rect rock_col = {
            .x = rock_pos.x + 10,
            .y = rock_pos.y + 10,
            .w = rock_pos.w - 20,
            .h = rock_pos.h - 60
        };
        _state.collision_rects[_state.collision_rect_ctr] = rock_col;
        ++_state.collision_rect_ctr;
    }

    // Add player
    Rect rect = {.x = _state.player.position.x, .y = _state.player.position.y, .w = PLAYER_W, .h = PLAYER_H};
    MCR_push_sprite("actor_guy", rect);
}


/**
 * Primary entry point to rendering.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 */
void render()
{
    // RGBColor outline = {.r = 255, .g = 0, .b = 0};
    // // Outline collision shapes
    // for(int i = 0; i < _state.collision_rect_ctr; ++i) {
    //     MCR_draw_rect(_state.collision_rects[i], outline);
    // }
}


/**
 * Primary entry point for keyboard input handling. This gets passed to
 * MCR_run as a callback.
 * 
 * INTPUT:
 * char -- Active key symbol
 * int  -- Boolean indication of key pressed down
 * 
 * OUTPUT: none
 */
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


/**
 * Primary entry point for mouse event handling.
 * 
 * INPUT:
 * uint32 -- Left (0) or Right (1) button
 * uint32 -- X coordinate
 * uint32 -- Y coordinate
 * uint32 -- Boolean indication of mouse button down
 * 
 * OUTPUT: none
 */
void mouse(uint32 button, uint32 x, uint32 y, uint32 down)
{
}


/**
 * Main entry to game client
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 */
int main()
{
    MCR_init("Micro Farm");
    MCR_set_tile_size(TILE_SIZE);

    int loaded = MCR_load_asset_class("assets\\actor", "actor");
    loaded = MCR_load_asset_class("assets\\world", "world");
    // loaded = MCR_load_asset_class("assets\\icons", "icon");
    // printf("Assets loaded from icon: %d\n", loaded);

    init_player();

    // TODO: I don't like passing an image path into this since we already loaded assets. But this needs an SDL_Surface
    MCR_set_cursor("assets\\icons\\cursor.png");

    MCR_run(&update, &render, &keyboard, &mouse);
}