#include "micro_engine.h"


typedef struct EditState {
    int show_collision;
} EditState;

EditState _edit_state;
State *_game_state;


/**
 * Toggles display of bounding boxes around collision rectangles.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 */
void toggle_collision_view()
{
    _edit_state.show_collision = !_edit_state.show_collision;
}


/**
 * Sets up editor with all its elements.
 * 
 * INPUT:
 * SDL_Renderer * -- Renderer stored in engine state
 * 
 * OUTPUT: none
 */
void init_editor(State *game_state)
{
    _game_state = game_state;
    load_asset_class(game_state->renderer, "res\\icons", "res_icon");

    int x = 10;
    int y = 10;
    DragContainer *container = create_container(x, y, 200, 500, "toolbar");

    int width, height;
    Button *button = create_button(x, y, 0, 0, "collision", "Collision");
    button->container = container;
    register_button_callback(button, &toggle_collision_view);

    get_button_dimensions(button, &width, &height);
    x += width + 10;
    button = create_button(x, y, 0, 0, "lights", "Lights");
    button->container = container;

    get_button_dimensions(button, &width, &height);
    x += width + 10;
    button = create_button(x, y, 0, 0, "paint", "Paint");
    button->container = container;
}


/**
 * Shows or hides the editor elements.
 * 
 * INPUT:
 * int -- Boolean indicating whether elements should be showing
 * 
 * OUTPUT: none
 */
void toggle_editor(int showing)
{
    DragContainer *dc = get_container_by_id("toolbar");
    if(dc) {
        dc->showing = showing;
    }
    Button *b = get_button_by_id("collision");
    if(b) {
        b->showing = showing;
    }
    b = get_button_by_id("lights");
    if(b) {
        b->showing = showing;
    }
    b = get_button_by_id("paint");
    if(b) {
        b->showing = showing;
    }
}


/**
 * Render conditional editor overlays.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 */
void render_edit_mode()
{
    if(_edit_state.show_collision) {
        RGBColor outline = {.r = 255, .g = 0, .b = 0};
        // Outline collision shapes
        for(int i = 0; i < _game_state->collision_objects.ctr; ++i) {
            MCR_draw_rect(_game_state->collision_objects.rects[i], outline);
        }
    }
}