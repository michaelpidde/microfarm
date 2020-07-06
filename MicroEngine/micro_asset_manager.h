#pragma once

#define MAX_ASSETS 100

typedef struct Assets {
    SDL_Texture *textures[MAX_ASSETS];
    char index[MAX_ASSETS][KEY_LENGTH];
    int ctr;
    SDL_Renderer *renderer;
} Assets;