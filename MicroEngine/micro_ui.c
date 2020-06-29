#include "micro_engine.h"

typedef struct RGBColor {
    unsigned int r;
    unsigned int g;
    unsigned int b;
} RGBColor;

typedef struct ButtonStyle {
    RGBColor base_color;
    RGBColor hover_color;
    RGBColor click_color;
    RGBColor border_color;
    unsigned int border_size;
    int text_alignment;
} ButtonStyle;

typedef enum ElementState {
    Off, Hover, Click
} ElementState;

#define MAX_BUTTON_TEXT 50
typedef struct Button {
    char text[MAX_BUTTON_TEXT];
    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
    ButtonStyle style;
    ElementState state;
} Button;

#define MAX_BUTTONS 20
typedef struct UI_State {
    Button buttons[MAX_BUTTONS];
    unsigned int button_ctr;
} UI_State;


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
    bs.text_alignment = 0;
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
void create_button(unsigned int x, unsigned int y, unsigned int w, unsigned int h, char *text)
{
    if(_ui_state.button_ctr < MAX_BUTTONS) {
        Button b;
        b.style = get_default_button_style();
        b.x = x;
        b.y = y;
        b.w = w;
        b.h = h;
        b.state = Off;
        strncpy(b.text, text, MAX_BUTTON_TEXT);
        _ui_state.buttons[_ui_state.button_ctr] = b;
        ++_ui_state.button_ctr;
    }
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
        // Border
        rect.x = button->x;
        rect.y = button->x;
        rect.w = button->w + (button->style.border_size * 2);
        rect.h = button->h + (button->style.border_size * 2);
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
        rect.w = button->w;
        rect.h = button->h;
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
        // Left, Right, Top, Bottom
        if(mouse_x >= b->x &&
           mouse_x <= (b->x + b->w + (b->style.border_size * 2)) &&
           mouse_y >= (b->y) &&
           mouse_y <= (b->y + b->h + (b->style.border_size * 2)))
        {
            if(gamestate->controls.mouse_left == 1) {
                b->state = Click;
            } else {
                b->state = Hover;
            }
        } else {
            b->state = Off;
        }
    }
}