#pragma once

#define Import __declspec(dllimport)
#define Export __declspec(dllexport)

// ASSETS
#define MAX_ASSETS 100
#define ASSET_KEY_LENGTH 50

#include "win32api.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>

Import 
int win32_get_directory_files(char **dest, char *dir, char *mask);

Import 
void win32_free_directory_list(char **src, int ctr);

typedef struct State {
    SDL_Window *window;
    SDL_Renderer *renderer;
} State;