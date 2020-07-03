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
    _ui_state.button_ctr = 0;
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
 * Add UI button.
 * 
 * INPUT:
 * unsigned int -- X position
 * unsigned int -- Y position
 * unsigned int -- Button width
 * unsigned int -- Button height
 * char *       -- Button text
 * 
 * OUTPUT: none
 ******************************************************************************/
void create_button(uint32 x, uint32 y, uint32 w, uint32 h, char *id, char *text)
{
    if(_ui_state.button_ctr < MAX_BUTTONS) {
        Button b;
        b.style = get_default_button_style();
        b.x = x;
        b.y = y;
        b.w = w;
        b.h = h;
        b.state = Off;
        strncpy(b.id, id, KEY_LENGTH);
        strncpy(b.text, text, MAX_BUTTON_TEXT);
        b.callback = NULL;
        _ui_state.buttons[_ui_state.button_ctr] = b;
        ++_ui_state.button_ctr;
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
    /*
     * Buttons
     */
    SDL_Rect rect;
    for(int i = 0; i < _ui_state.button_ctr; ++i) {
        Button *button = &_ui_state.buttons[i];

        // Set font before we start doing calculations on its size.
        set_font(button->style.font_key);

        int max_height, max_width;
        get_button_dimensions(button, &max_width, &max_height);

        // Border
        rect.x = button->x;
        rect.y = button->y;
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
        rect.x = button->x + button->style.border_size;
        rect.y = button->y + button->style.border_size;
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
        int x = button->x + button->style.border_size + button->style.padding;
        int y = button->y + button->style.border_size + button->style.padding;
        render_word(renderer, button->text, x, y);
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
     * Buttons
     */
    for(int i = 0; i < MAX_BUTTONS; ++i) {
        Button *b = &_ui_state.buttons[i];

        // Set font before we start doing calculations on its size.
        set_font(b->style.font_key);

        int max_width, max_height;
        get_button_dimensions(b, &max_width, &max_height);

        int left = b->x;
        int right = b->x + max_width;
        int top = b->y;
        int bottom = b->y + max_height;
        if(mouse_x >= left &&
           mouse_x <= right &&
           mouse_y >= top &&
           mouse_y <= bottom)
        {
            if(gamestate->controls.mouse_left == 1) {
                b->state = Click;
                if(b->callback && !b->doing_callback) {
                    b->doing_callback = 1;
                    b->callback();
                }
            } else {
                b->state = Hover;
                b->doing_callback = 0;
            }
        } else {
            b->state = Off;
        }
    }
}