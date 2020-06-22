#pragma once

#include <SDL.h>

typedef struct Assets {
    SDL_Texture *textures[MAX_ASSETS];
    char index[MAX_ASSETS][ASSET_KEY_LENGTH];
    int ctr;
} Assets;