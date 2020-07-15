#include "micro_engine.h"
#include "micro_ui_manager.h"

// Forward declare because I don't want to make a header because this is the only file consuming these functions...
void toggle_paint_tools(int showing);
void toggle_editor(int showing);


typedef struct EditState {
    int show_collision;
    int show_paint_tools;
    char palette_asset[KEY_LENGTH];
} EditState;

EditState _edit_state;
State *_game_state;


/*******************************************************************************
 * CALLBACKS
 ******************************************************************************/
void click_collision()
{
    _edit_state.show_collision = !_edit_state.show_collision;
}


void click_paint()
{
    _edit_state.show_paint_tools = !_edit_state.show_paint_tools;
    toggle_paint_tools(_edit_state.show_paint_tools);
}


void main_container_close()
{
    _game_state->edit_mode = 0;
    toggle_editor(0);
}


void paint_container_close()
{
    _edit_state.show_paint_tools = 0;
    toggle_paint_tools(0);
}


/*******************************************************************************
 * INITIALIZERS
 ******************************************************************************/
void init_main_container()
{
    Rect container_pos;
    container_pos.x = 10;
    container_pos.y = 10;
    container_pos.w = 200;
    container_pos.h = 500;
    DragContainer *container = create_container(container_pos, "main");
    container->close_callback = &main_container_close;

    int width, height;
    Rect element_pos;
    element_pos.x = container_pos.x;
    element_pos.y = container_pos.y;
    Button *button = create_button(element_pos, "collision", "Collision");
    button->container = container;
    register_button_callback(button, &click_collision);

    get_button_dimensions(button, &width, &height);
    element_pos.x += width + 10;
    button = create_button(element_pos, "lights", "Lights");
    button->container = container;

    get_button_dimensions(button, &width, &height);
    element_pos.x += width + 10;
    button = create_button(element_pos, "paint", "Paint");
    button->container = container;
    register_button_callback(button, &click_paint);

    get_button_dimensions(button, &width, &height);
    element_pos.x = container_pos.x;
    element_pos.y += height + 10;
    element_pos.w = 150;
    SelectBox *select = create_selectbox(element_pos, "collision_objects", 4);
    select->container = container;
    add_selectbox_element(select, "rock", "Rock", 0);
    add_selectbox_element(select, "table", "Table", 1);
    add_selectbox_element(select, "bush", "Bush", 0);
    add_selectbox_element(select, "furry_dog", "Furry Dog", 0);
    add_selectbox_element(select, "cosmic_lamp", "Cosmic Lamp", 0);
    add_selectbox_element(select, "aspen", "Aspen Tree", 0);
    // TODO: Maybe tie this into add_selectbox_element somehow so it automatically
    // regenerates the image instead of needing to remember to do it manually.
    generate_options_image(_game_state->renderer, select);
}


void init_paint_tools()
{
    // TileGrid *tilegrid = create_tilegrid("palette", _edit_state.palette_asset);
    
    DragContainer *main_container = get_container("main");
    Rect container_pos;
    container_pos.x = main_container->position.x + main_container->position.w + 10;
    container_pos.y = main_container->position.y;
    container_pos.w = 500;
    container_pos.h = 500;
    DragContainer *container = create_container(container_pos, "paint_tools");
    container->close_callback = &paint_container_close;

    toggle_paint_tools(0);
}


void init_editor(State *game_state)
{
    _game_state = game_state;
    load_asset_class(game_state->renderer, "res\\icons", "res_icon");

    init_main_container();
    init_paint_tools();
}


void toggle_paint_tools(int showing)
{
    DragContainer *dc = get_container("paint_tools");
    if(dc) {
        dc->showing = showing;
    }
}


/*******************************************************************************
 * TOGGLES
 ******************************************************************************/
void toggle_main_container(int showing)
{
    DragContainer *dc = get_container("main");
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


void toggle_editor(int show)
{
    toggle_main_container(show);
    if(!show) {
        _edit_state.show_paint_tools = 0;
        toggle_paint_tools(0);
    }
}


/*******************************************************************************
 * RENDERERS
 ******************************************************************************/
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


/*******************************************************************************
 * ETCETERA
 ******************************************************************************/
void register_palette(char *key)
{
    strcpy(_edit_state.palette_asset, key);
}