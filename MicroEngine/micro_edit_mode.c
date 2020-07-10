#include "micro_engine.h"


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

    Rect container_pos;
    container_pos.x = 10;
    container_pos.y = 10;
    container_pos.w = 200;
    container_pos.h = 500;
    DragContainer *container = create_container(container_pos, "toolbar");

    int width, height;
    Rect element_pos;
    element_pos.x = container_pos.x;
    element_pos.y = container_pos.y;
    Button *button = create_button(element_pos, "collision", "Collision");
    button->container = container;
    register_button_callback(button, &toggle_collision_bounding_boxes);

    get_button_dimensions(button, &width, &height);
    element_pos.x += width + 10;
    button = create_button(element_pos, "lights", "Lights");
    button->container = container;

    get_button_dimensions(button, &width, &height);
    element_pos.x += width + 10;
    button = create_button(element_pos, "paint", "Paint");
    button->container = container;

    get_button_dimensions(button, &width, &height);
    element_pos.x = container_pos.x;
    element_pos.y += height + 10;
    element_pos.w = 150;
    SelectBox *select = create_selectbox(element_pos, "collision_objects", 4);
    select->container = container;
    add_selectbox_element(select, "rock", "Rock", 1);
}


void toggle_editor(int showing)
{
    DragContainer *dc = get_container("toolbar");
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
    SelectBox *sb = get_selectbox("collision_objects");
    if(sb) {
        sb->showing = showing;
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