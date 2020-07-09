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


void MCR_set_tile_size(int size)
{
    _state.tile_size = size;
}


int MCR_load_asset_class(char *dir, char *prefix)
{
    return load_asset_class(_state.renderer, dir, prefix);
}


void render_spritebatch()
{
    SDL_SetRenderDrawColor(_state.renderer, 0, 0, 0, 255);

    SDL_Rect src;
    SDL_Rect dest; 
    for(int i = 0; i < _state.spritebatch.ctr; ++i) {
        src = to_SDL_Rect(_state.spritebatch.src[i]);
        dest = to_SDL_Rect(_state.spritebatch.dest[i]);
        SDL_RenderCopy(_state.renderer, get_asset(_state.spritebatch.keys[i]), &src, &dest);
    }
}


void clear_sprite_batch()
{
    _state.spritebatch.ctr = 0;
}


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


void render(void (*render_callback)())
{
    SDL_RenderClear(_state.renderer);

    render_spritebatch();
    render_callback();
    render_ui(_state.renderer);
    render_edit_mode();
    SDL_RenderPresent(_state.renderer);
}


void MCR_push_sprite(char *key, Rect src, Rect dest)
{
    if(_state.spritebatch.ctr < MAX_DRAW_BATCH) {
        strcpy(_state.spritebatch.keys[_state.spritebatch.ctr], key);
        _state.spritebatch.src[_state.spritebatch.ctr] = src;
        _state.spritebatch.dest[_state.spritebatch.ctr] = dest;
        ++_state.spritebatch.ctr;
    } else {
        // TODO: Log this or something
    }
}


void MCR_get_output_tiles(int *dimensions)
{
    SDL_GetRendererOutputSize(_state.renderer, &dimensions[0], &dimensions[1]);
    // Adding the divisor - 1 will prevent integer truncation.
    dimensions[0] = (dimensions[0] + (_state.tile_size - 1)) / _state.tile_size;
    dimensions[1] = (dimensions[1] + (_state.tile_size - 1)) / _state.tile_size;
}


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


void MCR_push_ui_button(Rect position, char *id, char *text)
{
    create_button(position, id, text);
}


void MCR_get_button_dimensions(char *id, int *max_width, int *max_height)
{
    Button *button = get_button(id);
    if(button) {
        get_button_dimensions(button, max_width, max_height);
    }
}


void MCR_register_button_callback(char *id, void (*callback)())
{
    Button *button = get_button(id);
    if(button) {
        register_button_callback(button, callback);
    }
}


void MCR_push_collision_object(Rect rect)
{
    _state.collision_objects.rects[_state.collision_objects.ctr] = rect;
    ++_state.collision_objects.ctr;
}


Rect *MCR_check_collision(Rect r1)
{
    for(int i = 0; i < _state.collision_objects.ctr; ++i) {
        if(rect_overlap(r1, _state.collision_objects.rects[i])) {
            return &_state.collision_objects.rects[i];
        }
    }

    return NULL;
}


void clear_collision()
{
    _state.collision_objects.ctr = 0;
}


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


void MRC_quit()
{
    _state.running = 0;
}


void update(void (*update_callback)())
{
    update_ui(&_state);
    update_callback();
}


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