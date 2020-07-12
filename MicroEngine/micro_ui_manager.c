#include "micro_ui_manager.h"

#include "cast.h"
#include "ui_elements/button.c"
#include "ui_elements/drag_container.c"
#include "ui_elements/selectbox.c"


UI_State _ui_state;


void init_UI()
{
    _ui_state.render_style = Adaptive;
}


Button *get_button(char *id)
{
    for(int i = 0; i < _ui_state.button_ctr; ++i) {
        if(strcmp(id, _ui_state.buttons[i].id) == 0) {
            return &_ui_state.buttons[i];
        }
    }
    // TODO: Figure out what to do if this doesn't return in the loop.
}


Button *create_button(Rect position, char *id, char *text)
{
    if(_ui_state.button_ctr < MAX_BUTTONS) {
        Button b = _create_button(position, id, text);
        _ui_state.buttons[_ui_state.button_ctr] = b;
        ++_ui_state.button_ctr;
        return &_ui_state.buttons[_ui_state.button_ctr - 1];
    } else {
        error("Max number of buttons added.\n");
    }
}


void get_button_dimensions(Button *button, int *max_width, int *max_height)
{
    _get_button_dimensions(_ui_state.render_style, button, max_width, max_height);
}


void get_selectbox_dimensions(SelectBox *sb, int *max_width, int *max_height)
{
    _get_selectbox_dimensions(_ui_state.render_style, sb, max_width, max_height);
}


DragContainer *create_container(Rect position, char *id)
{
    if(_ui_state.container_ctr < MAX_CONTAINERS) {
        DragContainer dc = _create_container(position, id);
        _ui_state.containers[_ui_state.container_ctr] = dc;
        ++_ui_state.container_ctr;
        return &_ui_state.containers[_ui_state.container_ctr - 1];
    } else {
        error("Max number of containers added.\n");
    }
}


DragContainer *get_container(char *id)
{
    for(int i = 0; i < _ui_state.container_ctr; ++i) {
        if(strcmp(id, _ui_state.containers[i].id) == 0) {
            return &_ui_state.containers[i];
        }
    }
    // TODO: Figure out what to do if this doesn't return in the loop.
}


SelectBox *create_selectbox(Rect position, char *id, int visible_elements)
{
    if(_ui_state.select_ctr < MAX_SELECT_ELEMENTS) {
        SelectBox sb = _create_selectbox(position, id, visible_elements);
        _ui_state.selectboxes[_ui_state.select_ctr] = sb;
        ++_ui_state.select_ctr;
        return &_ui_state.selectboxes[_ui_state.select_ctr - 1];
    } else {
        error("Max number of select boxes added.\n");
    }
}


SelectBox *get_selectbox(char *id) {
    for(int i = 0; i < _ui_state.select_ctr; ++i) {
        if(strcmp(id, _ui_state.selectboxes[i].id) == 0) {
            return &_ui_state.selectboxes[i];
        }
    }
    // TODO: Figure out what to do if this doesn't return in the loop.
}


void render_ui(SDL_Renderer *renderer)
{
    for(int i = 0; i < _ui_state.container_ctr; ++i) {
        render_container(renderer, &_ui_state.containers[i]);
    }

    for(int i = 0; i < _ui_state.button_ctr; ++i) {
        render_button(renderer, &_ui_state.buttons[i]);
    }

    for(int i = 0; i < _ui_state.button_ctr; ++i) {
        render_selectbox(renderer, &_ui_state.selectboxes[i]);
    }
}


void update_ui(State *gamestate)
{
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    /*
     * Containers
     */
    for(int i = 0; i < _ui_state.container_ctr; ++i) {
        DragContainer *container = &_ui_state.containers[i];
        if(container->showing) {
            Rect rect = get_drag_bar_position(container);
            int left = rect.x;
            int right = rect.x + rect.w;
            int top = rect.y;
            int bottom = rect.y + rect.h;
            // Check if state is Click so if we're dragging we can keep doing movement. If we only check if mouse
            // is inside dimensions of drag bar, if you're dragging wildly then you're going to mouse off of the
            // drag bar and it will stop tracking with the mouse movement even while the mouse button is still clicked.
            if(container->dragbar_state == Click ||
                (mouse_x >= left &&
                mouse_x <= right &&
                mouse_y >= top &&
                mouse_y <= bottom)
            ) {
                if(gamestate->controls.mouse_left == 1) {
                    container->dragbar_state = Click;
                    int move_x = _ui_state.last_mouse_x - mouse_x;
                    int move_y = _ui_state.last_mouse_y - mouse_y;
                    // move_x and move_y will be positive for up/left movement and negative for down/right movement. Ex:
                    // x = 100. Move left 5 pixels. x = 100 - 5.
                    // x = 100. Move right 5 pixels. x = 100 - (-5).
                    container->position.x -= move_x;
                    container->position.y -= move_y;
                } else {
                    container->dragbar_state = Hover;
                }
            } else {
                container->dragbar_state = Off;
            }
        }
    }

    /*
     * Buttons
     */
    for(int i = 0; i < MAX_BUTTONS; ++i) {
        Button *button = &_ui_state.buttons[i];
        if(button->showing) {
            // Set font before we start doing calculations on its size.
            set_font(button->style.font_key);

            int max_width, max_height;
            get_button_dimensions(button, &max_width, &max_height);

            // Real X and Y account for relative positioning inside of a container.
            int real_x = button->position.x;
            int real_y = button->position.y;
            if(button->container) {
                real_x = button->container->position.x + button->container->style.padding + button->position.x;
                real_y = button->container->position.y +
                    get_drag_bar_position(button->container).h +
                    button->container->style.padding +
                    button->position.y;
            }

            int left = real_x;
            int right = real_x + max_width;
            int top = real_y;
            int bottom = real_y + max_height;
            if(mouse_x >= left &&
                mouse_x <= right &&
                mouse_y >= top &&
                mouse_y <= bottom)
            {
                if(gamestate->controls.mouse_left == 1) {
                    button->state = Click;
                    if(button->callback && !button->doing_callback) {
                        button->doing_callback = 1;
                        button->callback();
                    }
                } else {
                    button->state = Hover;
                    button->doing_callback = 0;
                }
            } else {
                button->state = Off;
            }
        }
    }

    /*
     * Select Boxes
     */
    for(int i = 0; i < MAX_SELECT_ELEMENTS; ++i) {
        SelectBox *sb = &_ui_state.selectboxes[i];
        if(sb->showing) {
            // Set font before we start doing calculations on its size.
            set_font(sb->style.font_key);

            int max_width, max_height;
            get_selectbox_dimensions(sb, &max_width, &max_height);

            // Real X and Y account for relative positioning inside of a container.
            int real_x = sb->position.x;
            int real_y = sb->position.y;
            if(sb->container) {
                real_x = sb->container->position.x + sb->container->style.padding + sb->position.x;
                real_y = sb->container->position.y + sb->container->style.padding + sb->position.y;
            }

            int left = real_x;
            int right = real_x + max_width;
            int top = real_y;
            int bottom = real_y + max_height;
            if(mouse_x >= left &&
                mouse_x <= right &&
                mouse_y >= top &&
                mouse_y <= bottom)
            {
                if(gamestate->controls.mouse_left == 1) {
                    sb->state = Click;
                    if(sb->callback && !sb->doing_callback) {
                        sb->doing_callback = 1;
                        sb->callback();
                    }
                } else if(gamestate->controls.mouse_scroll != 0) {
                    // This will scroll the select box by one option element.
                    int next = sb->scroll_amount - (gamestate->controls.mouse_scroll * font_height());
                    int max_scroll_amount = sb->options_texture_h - max_height;
                    if(next < 0) {
                        next = 0;
                    }
                    if(next > max_scroll_amount) {
                        next = max_scroll_amount;
                    }
                    sb->scroll_amount = next;
                } else {
                    sb->state = Hover;
                    sb->doing_callback = 0;
                }
            } else {
                sb->state = Off;
            }
        }
    }

    // Save these to calculate drag.
    _ui_state.last_mouse_x = mouse_x;
    _ui_state.last_mouse_y = mouse_y;

    // Reset mouse scroll because this won't stop scrolling until we do this manually.
    gamestate->controls.mouse_scroll = 0;
}