#include "micro_ui_manager.h"


UI_State _ui_state;


/*******************************************************************************
 * Initialize UI state.
 * 
 * INPUT: 
 * 
 * OUTPUT: 
 ******************************************************************************/
void init_UI()
{
    _ui_state.render_style = Adaptive;
}


/*******************************************************************************
 * Gets generic color and style scheme for button.
 * 
 * INPUT: none
 * 
 * OUTPUT:
 * ButtonStyle -- Button style information
 ******************************************************************************/
ButtonStyle get_default_button_style()
{
    ButtonStyle bs;
    RGBColor base = {.r = 150, .g = 150, .b = 150};
    RGBColor hover = {.r = 120, .g = 120, .b = 120};
    RGBColor click = {.r = 150, .g = 100, .b = 100};
    RGBColor border = {.r = 0, .g = 0, .b = 0};
    bs.base_color = base;
    bs.hover_color = hover;
    bs.click_color = click;
    bs.border_color = border;
    bs.border_size = 2;
    bs.padding = 4;
    bs.text_alignment = -1;
    strcpy(bs.font_key, DEFAULT_UI_ELEMENT_FONT);
    return bs;
}


/*******************************************************************************
 * Gets generic color and style scheme for container.
 * 
 * INPUT: none
 * 
 * OUTPUT:
 * ContainerStyle -- Container style information
 ******************************************************************************/
ContainerStyle get_default_container_style()
{
    ContainerStyle cs;
    RGBColor base = {.r = 248, .g = 232, .b = 218};
    RGBColor border = {.r = 213, .g = 120, .b = 36};
    cs.base_color = base;
    cs.border_color = border;
    cs.border_size = 3;
    cs.padding = 7;
    return cs;
}


/*******************************************************************************
 * Add UI button.
 * 
 * INPUT:
 * unsigned int -- X position
 * unsigned int -- Y position
 * unsigned int -- Button width
 * unsigned int -- Button height
 * char *       -- Button text
 * 
 * OUTPUT:
 * Button *     -- Reference to created element
 ******************************************************************************/
Button *create_button(uint32 x, uint32 y, uint32 w, uint32 h, char *id, char *text)
{
    if(_ui_state.button_ctr < MAX_BUTTONS) {
        Button b;
        b.style = get_default_button_style();
        b.x = x;
        b.y = y;
        b.w = w;
        b.h = h;
        b.state = Off;
        b.showing = 1;
        strncpy(b.id, id, KEY_LENGTH);
        strncpy(b.text, text, MAX_BUTTON_TEXT);
        b.callback = NULL;
        _ui_state.buttons[_ui_state.button_ctr] = b;
        ++_ui_state.button_ctr;
        return &_ui_state.buttons[_ui_state.button_ctr - 1];
    } else {
        error("Max number of buttons added.\n");
    }
}


/*******************************************************************************
 * Add UI container.
 * 
 * INPUT:
 * unsigned int    -- X position
 * unsigned int    -- Y position
 * unsigned int    -- Button width
 * unsigned int    -- Button height
 * 
 * OUTPUT:
 * DragContainer * -- Reference to created element
 ******************************************************************************/
DragContainer *create_container(uint32 x, uint32 y, uint32 w, uint32 h, char *id)
{
    if(_ui_state.container_ctr < MAX_CONTAINERS) {
        DragContainer dc;
        dc.style = get_default_container_style();
        dc.x = x;
        dc.y = y;
        dc.w = w;
        dc.h = h;
        dc.showing = 1;
        dc.dragbar_state = Off;
        strcpy(dc.id, id);
        _ui_state.containers[_ui_state.container_ctr] = dc;
        ++_ui_state.container_ctr;
        return &_ui_state.containers[_ui_state.container_ctr - 1];
    } else {
        error("Max number of containers added.\n");
    }
}


/*******************************************************************************
 * Gets the max dimensions of a button based on the UI rendering style.
 * 
 * INPUT:
 * Button * -- Button to operate on
 * int *    -- Max width to write back to
 * int *    -- Max height to write back to
 * 
 * OUTPUT: none
 * 
 * WARNING: If set_font has not been called before this function it will
 * probably blow up.
 ******************************************************************************/
void get_button_dimensions(Button *button, int *max_width, int *max_height)
{
    // TODO: This is really stupid and I shouldn't have to do this here.
    set_font(button->style.font_key);

    int text_width = get_word_width(button->text);
    int text_height = *_selected_font->font_size;

    *max_width = button->w + (button->style.border_size * 2);
    *max_height = button->h + (button->style.border_size * 2);
    if(_ui_state.render_style == Adaptive) {
        *max_width = text_width + (button->style.border_size * 2) + (button->style.padding * 2);
        *max_height = text_height + (button->style.border_size * 2) + (button->style.padding * 2);
    }
}


/*******************************************************************************
 * Gets dimensions of drag bar portion of DragContainer element.
 * 
 * INPUT:
 * DragContainer * -- The container to target
 * 
 * OUTPUT:
 * Rect            -- The dimensions of the drag bar
 ******************************************************************************/
SDL_Rect get_drag_bar_position(DragContainer *container)
{
    SDL_Rect dimensions;
    dimensions.x = container->x + container->style.border_size;
    dimensions.y = container->y + container->style.border_size;
    dimensions.h = DRAG_BAR_HEIGHT;
    dimensions.w = container->w - (container->style.border_size * 2);
    return dimensions;
}


/*******************************************************************************
 * Find a button by its ID.
 * 
 * INPUT:
 * char * -- ID
 * 
 * OUTPUT:
 * Button -- Button element
 ******************************************************************************/
Button *get_button_by_id(char *id)
{
    for(int i = 0; i < _ui_state.button_ctr; ++i) {
        if(strcmp(id, _ui_state.buttons[i].id) == 0) {
            return &_ui_state.buttons[i];
        }
    }
    // TODO: Figure out what to do if this doesn't return in the loop.
}


/*******************************************************************************
 * Find a container by its ID.
 * 
 * INPUT:
 * char * -- ID
 * 
 * OUTPUT:
 * DragContainer -- Container element
 ******************************************************************************/
DragContainer *get_container_by_id(char *id)
{
    for(int i = 0; i < _ui_state.container_ctr; ++i) {
        if(strcmp(id, _ui_state.containers[i].id) == 0) {
            return &_ui_state.containers[i];
        }
    }
    // TODO: Figure out what to do if this doesn't return in the loop.
}


/*******************************************************************************
 * Sets callback function for button to call on click.
 * 
 * INPUT:
 * Button *   -- Button to associate callback to
 * void (*)() -- Function pointer to callback
 * 
 * OUTPUT: none
 ******************************************************************************/
void register_button_callback(Button *button, void (*callback)())
{
    button->callback = callback;
}


/*******************************************************************************
 * Render UI elements. This should happen after sprite batch rendering.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 ******************************************************************************/
void render_ui(SDL_Renderer *renderer)
{
    SDL_Rect rect;

    /*
     * Containers
     */
    for(int i = 0; i < _ui_state.container_ctr; ++i) {
        DragContainer *container = &_ui_state.containers[i];
        if(container->showing) {
            // Border
            rect.x = container->x;
            rect.y = container->y;
            rect.h = container->h;
            rect.w = container->w;
            SDL_SetRenderDrawColor(
                renderer,
                container->style.border_color.r,
                container->style.border_color.g,
                container->style.border_color.b,
                255
            );
            SDL_RenderFillRect(renderer, &rect);

            // Body
            rect.x = container->x + container->style.border_size;
            rect.y = container->y + container->style.border_size;
            rect.w = container->w - (container->style.border_size * 2);
            rect.h = container->h - (container->style.border_size * 2);
            SDL_SetRenderDrawColor(
                renderer,
                container->style.base_color.r,
                container->style.base_color.g,
                container->style.base_color.b,
                255
            );
            SDL_RenderFillRect(renderer, &rect);

            // Drag bar
            SDL_Rect rect = get_drag_bar_position(container);
            SDL_Rect src;
            src.x = 0;
            src.y = 0;
            int w, h;
            SDL_Texture *texture = get_asset("res_icon_drag_panel_head");
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            src.w = w;
            src.h = h;
            SDL_RenderCopy(renderer, texture, &src, &rect);
        }
    }

    /*
     * Buttons
     */
    for(int i = 0; i < _ui_state.button_ctr; ++i) {
        Button *button = &_ui_state.buttons[i];
        if(button->showing) {
            // Real X and Y account for relative positioning inside of a container.
            int real_x = 0;
            int real_y = 0;
            if(button->container) {
                real_x = button->container->x + button->container->style.padding;
                real_y = button->container->y +
                    get_drag_bar_position(button->container).h +
                    button->container->style.padding;
            }

            // Set font before we start doing calculations on its size.
            set_font(button->style.font_key);

            int max_height, max_width;
            get_button_dimensions(button, &max_width, &max_height);

            // Border
            rect.x = real_x + button->x;
            rect.y = real_y + button->y;
            rect.w = max_width;
            rect.h = max_height;
            SDL_SetRenderDrawColor(
                renderer,
                button->style.border_color.r,
                button->style.border_color.g,
                button->style.border_color.b,
                255
            );
            SDL_RenderFillRect(renderer, &rect);

            // Body
            rect.x = real_x + button->x + button->style.border_size;
            rect.y = real_y + button->y + button->style.border_size;
            rect.w = max_width - (button->style.border_size * 2);
            rect.h = max_height - (button->style.border_size * 2);
            RGBColor *state_color = &button->style.base_color;
            switch(button->state) {
                case Hover: {
                    state_color = &button->style.hover_color;
                } break;
                case Click: {
                    state_color = &button->style.click_color;
                } break;
            }
            SDL_SetRenderDrawColor(
                renderer,
                state_color->r,
                state_color->g,
                state_color->b,
                255
            );
            SDL_RenderFillRect(renderer, &rect);

            // TODO: Handle clipping for Strict RenderStyle
            // Text
            SDL_Rect dest;
            int x = real_x + button->x + button->style.border_size + button->style.padding;
            int y = real_y + button->y + button->style.border_size + button->style.padding;
            render_word(renderer, button->text, x, y);
        }
    }
}


/*******************************************************************************
 * Update UI elements. Check for mouse position and change element states
 * based on it.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 ******************************************************************************/
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
            SDL_Rect rect = get_drag_bar_position(container);
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
                    container->x -= move_x;
                    container->y -= move_y;
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
            int real_x = button->x;
            int real_y = button->y;
            if(button->container) {
                real_x = button->container->x + button->container->style.padding + button->x;
                real_y = button->container->y +
                    get_drag_bar_position(button->container).h +
                    button->container->style.padding +
                    button->y;
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

    // Save these to calculate drag.
    _ui_state.last_mouse_x = mouse_x;
    _ui_state.last_mouse_y = mouse_y;
}