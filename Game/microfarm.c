#include "microfarm.h"

State _state;

/*******************************************************************************
 * Single compilation unit.
 ******************************************************************************/
#include "unitybuild.c"


/*******************************************************************************
 * Primary entry point for game state updating. This gets passed into
 * MCR_run as a callback.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 ******************************************************************************/
void update_callback()
{
    // Set background
    int dimensions[2] = {0, 0};
    MCR_get_output_tiles(dimensions);

    for(int row = 0; row < dimensions[1]; ++row) {
        for(int col = 0; col < dimensions[0]; ++col) {
            Rect r = {.x = col * TILE_SIZE, .y = row * TILE_SIZE, .w = TILE_SIZE, .h = TILE_SIZE};
            MCR_push_sprite("world_terrain", r);
        }
    }

    Rect rect = {.x = _state.player.position.x, .y = _state.player.position.y, .w = 32, .h = 64};
    MCR_push_sprite("actor_guy", rect);
}


/*******************************************************************************
 * Primary entry point for keyboard input handling. This gets passed to
 * MCR_run as a callback.
 * 
 * INTPUT:
 * char -- Active key symbol
 * int  -- Boolean indication of key pressed down
 * 
 * OUTPUT: none
 ******************************************************************************/
void keyboard_callback(char sym, int down)
{
    switch(sym) {
        case KEY_q: {
            MRC_quit();
        } break;
        case KEY_w: {
            player_move(North);
        } break;
        case KEY_a: {
            player_move(West);
        } break;
        case KEY_s: {
            player_move(South);
        } break;
        case KEY_d: {
            player_move(East);
        } break;
    }
}


/*******************************************************************************
 * Primary entry point for mouse event handling.
 * 
 * INPUT:
 * uint32 -- Left (0) or Right (1) button
 * uint32 -- X coordinate
 * uint32 -- Y coordinate
 * uint32 -- Boolean indication of mouse button down
 * 
 * OUTPUT: none
 ******************************************************************************/
void mouse_callback(uint32 button, uint32 x, uint32 y, uint32 down)
{
    char b[6] = "left";
    if(button == 1) {
        strcpy(b, "right");
    }
    printf("MOUSE: %d,%d, %s\n", x, y, b);
}


/*******************************************************************************
 * Main entry to game client
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 ******************************************************************************/
int main()
{
    MCR_init();
    MCR_set_tile_size(TILE_SIZE);

    int loaded = MCR_load_asset_class("assets\\actor", "actor");
    printf("Assets loaded actor: %d\n", loaded);
    loaded = MCR_load_asset_class("assets\\world", "world");
    printf("Assets loaded from world: %d\n", loaded);
    // loaded = MCR_load_asset_class("assets\\icons", "icon");
    // printf("Assets loaded from icon: %d\n", loaded);

    init_player();

    // TODO: I don't like passing an image path into this since we already loaded assets. But this needs an SDL_Surface
    MCR_set_cursor("assets\\icons\\cursor.png");

    // TODO: Remove this, just testing
    MCR_push_ui_button(300, 300, 100, 25, "Test\nButton");

    MCR_run(&update_callback, &keyboard_callback, &mouse_callback);
}