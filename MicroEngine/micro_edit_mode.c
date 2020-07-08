#include "micro_engine.h"
#include "ui_elements/drag_container.h"


typedef struct EditState {
    int show_collision;
} EditState;

EditState _edit_state;
State *_game_state;


void toggle_collision_bounding_boxes()
{
    _edit_state.show_collision = !_edit_state.show_collision;
}


void init_editor(State *game_state)
{
    _game_state = game_state;
    load_asset_class(game_state->renderer, "res\\icons", "res_icon");

    Rect position;
    position.x = 10;
    position.y = 10;
    position.w = 200;
    position.h = 500;
    DragContainer *container = create_container(position, "toolbar");

    int width, height;
    Rect button_position;
    button_position.x = position.x;
    button_position.y = position.y;
    Button *button = create_button(button_position, "collision", "Collision");
    button->container = container;
    register_button_callback(button, &toggle_collision_bounding_boxes);

    get_button_dimensions(button, &width, &height);
    button_position.x += width + 10;
    button = create_button(button_position, "lights", "Lights");
    button->container = container;

    get_button_dimensions(button, &width, &height);
    button_position.x += width + 10;
    button = create_button(button_position, "paint", "Paint");
    button->container = container;
}


void toggle_editor(int showing)
{
    DragContainer *dc = get_container_by_id("toolbar");
    if(dc) {
        dc->showing = showing;
    }
    Button *b = get_button("collision");
    if(b) {
        b->showing = showing;
    }
    b = get_button("lights");
    if(b) {
        b->showing = showing;
    }
    b = get_button("paint");
    if(b) {
        b->showing = showing;
    }
}


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