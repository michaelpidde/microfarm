#pragma once

#define Import __declspec(dllimport)
#define Export __declspec(dllexport)

#ifdef MCR_APP
    #define ImEx Import
#else
    #define ImEx Export
#endif

#include "../Common/types.h"

ImEx
void MCR_init(char *title);
ImEx
void MCR_set_tile_size(int size);
ImEx
int MCR_load_asset_class(char *dir, char *prefix);
ImEx
void MCR_run(
    void (*update_callback)(), 
    void (*render_callback)(),
    void (*keyboard_callback)(char sym, int down), 
    void (*mouse_callback)(uint32 button, uint32 x, uint32 y, uint32 down));
ImEx
void MCR_push_sprite(char *key, Rect rect);
ImEx
void MCR_get_output_tiles(int *dimensions);
ImEx
void MCR_set_cursor(char *icon);
ImEx
void MRC_quit();
ImEx
void MCR_draw_rect(Rect rect, RGBColor color);
ImEx
Rect *MCR_check_collision(Rect r1);
ImEx
void MCR_push_collision_object(Rect rect);

/*
 * UI
 */
#include "micro_ui_manager.h"
ImEx
void MCR_push_ui_button(uint32 x, uint32 y, uint32 w, uint32 h, char *id, char *text);
ImEx
void MCR_get_button_dimensions(char *id, int *max_width, int *max_height);
ImEx
void MCR_register_button_callback(char *id, void (*callback)());