#pragma once
#include "../Common/keys.h"
#include "../Common/types.h"
#include "types.h"
#include "utility.h"
#include <stdio.h>
#include <string.h>

#define Import __declspec(dllimport)
#define TILE_SIZE 32

Import
void MCR_init();
Import
void MCR_set_tile_size(int size);
Import
void MCR_load_font(char *path, char *key);
Import
int MCR_load_asset_class(char *dir, char *prefix);
Import
void MCR_run(
    void (*update_callback)(), 
    void (*keyboard_callback)(char sym, int down), 
    void (*mouse_callback)(uint32 button, uint32 x, uint32 y, uint32 down));
Import
void MCR_push_sprite(char *key, Rect rect);
Import
void MCR_get_output_tiles(int *dimensions);
Import
void MCR_set_cursor(char *icon);
Import
void MRC_quit();

/*
 * UI
 */
Import
void MCR_push_ui_button(uint32 x, uint32 y, uint32 w, uint32 h, char *text);