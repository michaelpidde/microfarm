#include "micro_engine.h"

#include "micro_debug.c"
#include "micro_asset_manager.c"
#if DEBUG
    #include "micro_edit_mode.c"
#endif
#include "micro_font_manager.c"
#include "micro_math.c"
#include "micro_ui_manager.c"

State _state;


/**
 * Sets up game window and renderer.
 * 
 * INPUT:
 * State * -- Primary engine state
 * char *  -- Title of game
 * 
 * OUTPUT:
 * int     -- Boolean success flag
 */
int init_window(State *state, char *title)
{
    int success = 1;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL. Error: %s\n", SDL_GetError());
        success = 0;
    } else {
        SDL_Rect display_bounds;
        display_bounds.x = 0;
        display_bounds.y = 0;

        #if DEBUG
        // I only care about putting the game on a second display during development.
        // Put it on the primary display otherwise.
        int displays = SDL_GetNumVideoDisplays();
        if(displays == 2) {
            SDL_GetDisplayBounds(1, &display_bounds);
            SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
        }
        #endif

        state->window = SDL_CreateWindow(
            title,
        #if DISPLAY_WINDOWED
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
        #else
            display_bounds.x,
            display_bounds.y,
        #endif
            800,
            600,
        #if DISPLAY_WINDOWED
            SDL_WINDOW_RESIZABLE
        #else
            SDL_WINDOW_FULLSCREEN_DESKTOP
        #endif
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


/**
 * Engine initialization. Proxy to initialize modules.
 * 
 * INPUT:
 * char * -- Title of game
 * 
 * OUTPUT: none
 */
void MCR_init(char *title)
{
    init_window(&_state, title);
    init_asset_manager(_state.renderer);
    init_font(_state.renderer);
    init_UI();
    #if DEBUG
        init_editor(&_state);
        toggle_editor(_state.edit_mode);
    #endif
    _state.spritebatch.ctr = 0;
    _state.tile_size = DEFAULT_TILE_SIZE;
}


/**
 * Allow override of default tile size.
 * 
 * INPUT:
 * int -- Override tile size
 * 
 * OUTPUT: none
 */
void MCR_set_tile_size(int size)
{
    _state.tile_size = size;
}


/**
 * Proxy to asset loader so client does not need to be aware of renderer.
 * 
 * INPUT:
 * char * -- Directory path to load assets from
 * char * -- Prefix for unique keys to reference assets by
 * 
 * OUTPUT:
 * int    -- Number of assets loaded
 */
int MCR_load_asset_class(char *dir, char *prefix)
{
    return load_asset_class(_state.renderer, dir, prefix);
}


/**
 * Sprite batch rendering. Clear to black, then loop sprite batch and render
 * each item.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 */
void render_spritebatch()
{
    SDL_SetRenderDrawColor(_state.renderer, 0, 0, 0, 255);

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
}


/**
 * Resets sprite batch. Batch arrays are not cleared, just reset the counter
 * to overrite array positions.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 */
void clear_sprite_batch()
{
    _state.spritebatch.ctr = 0;
}


/**
 * Draws rectangle to screen.
 * 
 * INPUT:
 * Rect     -- Rectangle coordinates
 * RGBColor -- Rectangle line color
 * 
 * OUTPUT: none
 */
void MCR_draw_rect(Rect rect, RGBColor color)
{
    SDL_SetRenderDrawColor(_state.renderer, color.r, color.g, color.b, 255);
    SDL_Rect r;
    r.x = rect.x;
    r.y = rect.y;
    r.w = rect.w;
    r.h = rect.h;
    SDL_RenderDrawRect(_state.renderer, &r);
}


/**
 * Main rendering entry point. Renders elements in proper order.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 */
void render(void (*render_callback)())
{
    SDL_RenderClear(_state.renderer);

    render_spritebatch();
    render_callback();
    render_ui(_state.renderer);
    render_edit_mode();
    SDL_RenderPresent(_state.renderer);
}


/**
 * Adds element to sprite batch.
 * 
 * INPUT:
 * char * -- Unique key of asset
 * Rect   -- Position rect where sprite will be rendered on screen
 * 
 * OUTPUT: none
 */
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


/**
 * Sets pointer with total number of tiles that can be rendered in X and Y 
 * planes.
 * 
 * INPUT:
 * int * -- Pointer to int[2] where index 0 is X and index 1 is Y
 * 
 * OUTPUT: none
 */
void MCR_get_output_tiles(int *dimensions)
{
    SDL_GetRendererOutputSize(_state.renderer, &dimensions[0], &dimensions[1]);
    // Adding the divisor - 1 will prevent integer truncation.
    dimensions[0] = (dimensions[0] + (_state.tile_size - 1)) / _state.tile_size;
    dimensions[1] = (dimensions[1] + (_state.tile_size - 1)) / _state.tile_size;
}


/**
 * Allow default system cursor to be overridden.
 * 
 * INPUT:
 * char * -- Path to image
 * 
 * OUTPUT: none
 */
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


/**
 * Proxy to add UI button
 * 
 * See micro_ui_manager::push_ui_button
 */
void MCR_push_ui_button(uint32 x, uint32 y, uint32 w, uint32 h, char *id, char *text)
{
    create_button(x, y, w, h, id, text);
}


/**
 * Proxy to get button dimensions.
 * 
 * INPUT:
 * char * -- ID of button
 * int *  -- Max render width to write back to
 * int *  -- Max render height to write back to
 * 
 * OUTPUT: none
 */
void MCR_get_button_dimensions(char *id, int *max_width, int *max_height)
{
    Button *button = get_button_by_id(id);
    if(button) {
        get_button_dimensions(button, max_width, max_height);
    }
}


/**
 * Proxy to register button callback, associating button by ID.
 * 
 * INPUT:
 * char * -- Unique ID of button
 * void * -- Callback for button click event
 * 
 * OUTPUT: none
 */
void MCR_register_button_callback(char *id, void (*callback)())
{
    Button *button = get_button_by_id(id);
    if(button) {
        register_button_callback(button, callback);
    }
}


/**
 * Adds element to collision object collection.
 * 
 * INPUT:
 * Rect -- Collision rectangle
 * 
 * OUTPUT: none
 */
void MCR_push_collision_object(Rect rect)
{
    _state.collision_objects.rects[_state.collision_objects.ctr] = rect;
    ++_state.collision_objects.ctr;
}


/**
 * Determines if player is going to collide with a world object in next
 * movement in given direction.
 * 
 * INPUT:
 * Rect      -- Rect to calculate collision against
 * 
 * OUTPUT:
 * Rect *    -- World object collided with
 */
Rect *MCR_check_collision(Rect r1)
{
    for(int i = 0; i < _state.collision_objects.ctr; ++i) {
        if(rect_overlap(r1, _state.collision_objects.rects[i])) {
            return &_state.collision_objects.rects[i];
        }
    }

    return NULL;
}


/**
 * Resets collision collection. Collision rect array is not cleared, just reset the counter
 * to overrite array positions.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 */
void clear_collision()
{
    _state.collision_objects.ctr = 0;
}


/**
 * Primary entry point for input device event handling.
 * 
 * INPUT:
 * SDL_Event -- Event container
 * void *    -- Callback for client keyboard handling
 * void *    -- Callback for client mouse handling
 * 
 * OUTPUT: none
 */
void handle_events(
    SDL_Event event, 
    void (*keyboard_callback)(char sym, int down),
    void (*mouse_callback)(uint32 button, uint32 x, uint32 y, uint32 down))
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
                switch(event.key.keysym.sym) {
                    #if DEBUG
                    case SDLK_BACKQUOTE: {
                        if(event.key.keysym.mod & KMOD_LALT || event.key.keysym.mod & KMOD_RALT) {
                            _state.edit_mode = !_state.edit_mode;
                            toggle_editor(_state.edit_mode);
                        }
                    } break;
                    #endif
                }

                keyboard_callback(event.key.keysym.sym, 1);
            } break;
            case SDL_MOUSEBUTTONDOWN: {
                uint32 button = 0;
                if(event.button.button == SDL_BUTTON_RIGHT) {
                    button = 1;
                }

                if(button == 1) {
                    _state.controls.mouse_right = 1;
                } else {
                    _state.controls.mouse_left = 1;
                }

                mouse_callback(button, event.button.x, event.button.y, 1);
            } break;
            case SDL_MOUSEBUTTONUP: {
                uint32 button = 0;
                if(event.button.button == SDL_BUTTON_RIGHT) {
                    button = 1;
                }

                if(button == 1) {
                    _state.controls.mouse_right = 0;
                } else {
                    _state.controls.mouse_left = 0;
                }
            } break;
        }
    }
}


/**
 * Quit event used by client to end game loop.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 */
void MRC_quit()
{
    _state.running = 0;
}


/**
 * Main entry for subsystem updates.
 * 
 * INPUT:
 * void * -- Callback function to client update process
 * 
 * OUTPUT: none
 */
void update(void (*update_callback)())
{
    update_ui(&_state);
    update_callback();
}


/**
 * Main entry point into engine.
 * 
 * INPUT:
 * void * -- Callback for client to update its state
 * void * -- Callback for client to handle keyboard input
 * 
 * OUTPUT: none
 */
void MCR_run(
    void (*update_callback)(), 
    void (*render_callback)(),
    void (*keyboard_callback)(char sym, int down), 
    void (*mouse_callback)(uint32 button, uint32 x, uint32 y, uint32 down)) 
{
    _state.running = 1;

    SDL_Event event;
    // TODO: This is a really horrible game loop. Fix it.
    double freq = win32_timer_frequency();
    double old_time = win32_get_time(freq);
    double accumulator;
    double start;
    double frame_time = 16.33;
    while(_state.running) {
        start = win32_get_time(freq);
        accumulator += (start - old_time);
        old_time = start;

        handle_events(event, keyboard_callback, mouse_callback);
        
        if(accumulator > frame_time) {
            accumulator -= frame_time;
            update(update_callback);
            render(render_callback);
        } else {
            Sleep(1);
        }

        clear_sprite_batch();
        clear_collision();
    }

    SDL_DestroyRenderer(_state.renderer);
    SDL_DestroyWindow(_state.window);
    SDL_FreeCursor(_state.cursor);
    // TODO: Free assets
    SDL_Quit();
}

// TODO: Arena memory strategy?