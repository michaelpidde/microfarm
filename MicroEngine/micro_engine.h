#pragma once

#include "constants.h"
#include "MCR_API.h"

#include "..\Common\win32api.h"
#include "..\Common\types.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_TILE_SIZE 32
#define MAX_DRAW_BATCH 3000
#define DEBUG 1

Import 
int win32_get_directory_files(char **dest, char *dir, char *mask);
Import 
void win32_free_directory_list(char **src, int ctr);
Import
__int64 win32_timer_frequency();
Import
__int64 win32_query_performance_counter();

typedef struct SpriteBatch {
    char keys[MAX_DRAW_BATCH][KEY_LENGTH];
    Rect locations[MAX_DRAW_BATCH];
    int ctr;
} SpriteBatch;

typedef struct Controls {
    int mouse_left;
    int mouse_right;
} Controls;

typedef struct State {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Cursor *cursor;
    int running;
    SpriteBatch spritebatch;
    int tile_size;
    Controls controls;
} State;