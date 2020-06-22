#include "micro_engine.h"

#include "micro_asset_manager.c"

State _state;

int init_window(State *state)
{
    int success = 1;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL. Error: %s\n", SDL_GetError());
        success = 0;
    } else {
        state->window = SDL_CreateWindow(
            "Boreal Darkness",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            800,
            600,
            0
        );

        if(state->window == NULL) {
            printf("Failed to create game window. Error: %s\n", SDL_GetError());
            success = 0;
        } else {
            int img_flags = IMG_INIT_PNG;
            if((!IMG_Init(img_flags)) & img_flags) {
                printf("Failed to initialize SDL_Image. Error: %s\n", IMG_GetError());
                success = 0;
            } else {
                state->renderer = SDL_CreateRenderer(state->window, -1, 0);
            }
        }
    }

    return success;
}

Export
int ME_init()
{
    init_window(&_state);
    init_asset_manager(&_state);
    return 0;
}

Export
int ME_load_asset_class(char *dir, char *prefix)
{
    return load_asset_class(_state.renderer, dir, prefix);
}

// TODO: Arena memory strategy?