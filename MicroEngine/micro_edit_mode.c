#include "micro_engine.h"


/*******************************************************************************
 * Sets up editor with all its elements.
 * 
 * INPUT:
 * SDL_Renderer * -- Renderer stored in engine state
 * 
 * OUTPUT: none
 ******************************************************************************/
void init_editor(SDL_Renderer *renderer)
{
    load_asset_class(renderer, "res\\icons", "res_icon");

    int x = 10;
    int y = 10;
    DragContainer *container = create_container(x, y, 200, 500, "toolbar");

    int width, height;
    Button *button = create_button(x, y, 0, 0, "collision", "Collision");
    button->container = container;
    get_button_dimensions(button, &width, &height);
    x += width + 10;
    button = create_button(x, y, 0, 0, "lights", "Lights");
    button->container = container;
    // register_button_callback(button, &stupid_callback);
    get_button_dimensions(button, &width, &height);
    x += width + 10;
    button = create_button(x, y, 0, 0, "paint", "Paint");
    button->container = container;
}


/*******************************************************************************
 * Shows or hides the editor elements.
 * 
 * INPUT:
 * int -- Boolean indicating whether elements should be showing
 * 
 * OUTPUT: none
 ******************************************************************************/
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