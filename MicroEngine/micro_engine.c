#include "micro_asset_manager.c"
#include "micro_engine.h"

State _state;


/*******************************************************************************
 * Sets up game window and renderer.
 * 
 * INPUT:
 * State * -- Primary engine state
 * 
 * OUTPUT:
 * int -- Boolean success flag
 ******************************************************************************/
int init_window(State *state)
{
    int success = 1;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL. Error: %s\n", SDL_GetError());
        success = 0;
    } else {
        state->window = SDL_CreateWindow(
            "Micro Engine Game",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            800,
            600,
            SDL_WINDOW_FULLSCREEN_DESKTOP
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


/*******************************************************************************
 * Engine initialization. Proxy to initialize modules.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 ******************************************************************************/
Export
void MCR_init()
{
    init_window(&_state);
    init_asset_manager(&_state);
    _state.spritebatch.ctr = 0;
    _state.tile_size = DEFAULT_TILE_SIZE;
}


/*******************************************************************************
 * Allow override of default tile size.
 * 
 * INPUT:
 * int -- Override tile size
 * 
 * OUTPUT: none
 ******************************************************************************/
Export
void MCR_set_tile_size(int size)
{
    _state.tile_size = size;
}


/*******************************************************************************
 * Proxy to asset loader so client does not need to be aware of renderer.
 * 
 * INPUT:
 * char * -- Directory path to load assets from
 * char * -- Prefix for unique keys to reference assets by
 * 
 * OUTPUT:
 * int    -- Number of assets loaded
 ******************************************************************************/
Export
int MCR_load_asset_class(char *dir, char *prefix)
{
    return load_asset_class(_state.renderer, dir, prefix);
}


/*******************************************************************************
 * Primary render method. Clear to black, then loop sprite batch and render
 * each item.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 ******************************************************************************/
void render_spritebatch()
{
    SDL_SetRenderDrawColor(_state.renderer, 0, 0, 0, 255);
    SDL_RenderClear(_state.renderer);

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    SDL_Rect dest; 
    for(int i = 0; i < _state.spritebatch.ctr; ++i) {
        src.w = _state.spritebatch.locations[i].w;
        src.h = _state.spritebatch.locations[i].h;
        dest.x = _state.spritebatch.locations[i].x;
        dest.y = _state.spritebatch.locations[i].y;
        dest.w = _state.spritebatch.locations[i].w;
        dest.h = _state.spritebatch.locations[i].h;
        SDL_RenderCopy(_state.renderer, get_asset(_state.spritebatch.keys[i]), &src, &dest);
    }

    SDL_RenderPresent(_state.renderer);
}

void clear_sprite_batch()
{
    _state.spritebatch.ctr = 0;
}


/*******************************************************************************
 * Adds element to sprite batch.
 * 
 * INPUT:
 * char * -- Unique key of asset
 * Rect   -- Position rect where sprite will be rendered on screen
 * 
 * OUTPUT: none
 ******************************************************************************/
Export
void MCR_push_sprite(char *key, Rect rect)
{
    if(_state.spritebatch.ctr < MAX_DRAW_BATCH) {
        strcpy(_state.spritebatch.keys[_state.spritebatch.ctr], key);
        _state.spritebatch.locations[_state.spritebatch.ctr] = rect;
        ++_state.spritebatch.ctr;
    } else {
        // TODO: Log this or something
    }
}


/*******************************************************************************
 * Sets pointer with total number of tiles that can be rendered in X and Y 
 * planes.
 * 
 * INPUT:
 * int * -- Pointer to int[2] where index 0 is X and index 1 is Y
 * 
 * OUTPUT: none
 ******************************************************************************/
Export
void MCR_get_output_tiles(int *dimensions)
{
    SDL_GetRendererOutputSize(_state.renderer, &dimensions[0], &dimensions[1]);
    // Adding the divisor - 1 will prevent integer truncation.
    dimensions[0] = (dimensions[0] + (_state.tile_size - 1)) / _state.tile_size;
    dimensions[1] = (dimensions[1] + (_state.tile_size - 1)) / _state.tile_size;
}


/*******************************************************************************
 * Allow default system cursor to be overridden.
 * 
 * INPUT:
 * char * -- Path to image
 * 
 * OUTPUT: none
 ******************************************************************************/
Export
void MCR_set_cursor(char *image)
{
    SDL_Surface *loading_surface;
    loading_surface = IMG_Load(image);
    if(loading_surface != NULL) {
        _state.cursor = SDL_CreateColorCursor(loading_surface, 0, 0);
        SDL_SetCursor(_state.cursor);
    } else {
        printf("Error loading cursor: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(loading_surface);
}


/*******************************************************************************
 * Primary entry point for input device event handling.
 * 
 * INPUT:
 * SDL_Event -- Event container
 * void *    -- Callback for client keyboard handling
 * void *    -- Callback for client mouse handling
 * 
 * OUTPUT: none
 ******************************************************************************/
void handle_events(SDL_Event event, 
                   void (*keyboard_callback)(char sym, int down),
                   void (*mouse_callback)(unsigned int button, int x, int y))
{
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT: {
                _state.running = 0;
            } break;
            case SDL_KEYUP: {
                keyboard_callback(event.key.keysym.sym, 0);
            } break;
            case SDL_KEYDOWN: {
                keyboard_callback(event.key.keysym.sym, 1);
            } break;
            case SDL_MOUSEBUTTONDOWN: {
                unsigned int button = 0;
                if(event.button.button == SDL_BUTTON_RIGHT) {
                    button = 1;
                }
                mouse_callback(button, event.button.x, event.button.y);
            }
        }
    }
}


/*******************************************************************************
 * Quit event used by client to end game loop.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 ******************************************************************************/
Export
void MRC_quit()
{
    _state.running = 0;
}


/*******************************************************************************
 * Main entry point into engine.
 * 
 * INPUT:
 * void * -- Callback for client to update its state
 * void * -- Callback for client to handle keyboard input
 * 
 * OUTPUT: none
 ******************************************************************************/
Export
void MCR_run(void (*update_callback)(), 
             void (*keyboard_callback)(char sym, int down), 
             void (*mouse_callback)(unsigned int button, int x, int y)) 
{
    _state.running = 1;

    SDL_Event event;
    while(_state.running) {
        handle_events(event, keyboard_callback, mouse_callback);
        update_callback();
        render_spritebatch();
        clear_sprite_batch();
    }

    SDL_DestroyRenderer(_state.renderer);
    SDL_DestroyWindow(_state.window);
    SDL_FreeCursor(_state.cursor);
    // TODO: Free assets
    SDL_Quit();
}

// TODO: Arena memory strategy?