#include "button.h"
#include "drag_container.h"


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


Button _create_button(Rect position, char *id, char *text)
{
    Button b;
    b.style = get_default_button_style();
    b.position = position;
    b.state = Off;
    b.showing = 1;
    strncpy(b.id, id, KEY_LENGTH);
    strncpy(b.text, text, MAX_BUTTON_TEXT);
    b.callback = NULL;
    return b;
}


void _get_button_dimensions(RenderStyle render_style, Button *button, int *max_width, int *max_height)
{
    // TODO: This is really stupid and I shouldn't have to do this here.
    set_font(button->style.font_key);

    int text_width = get_word_width(button->text);
    int text_height = *_selected_font->font_size;

    *max_width = button->position.x + (button->style.border_size * 2);
    *max_height = button->position.h + (button->style.border_size * 2);
    if(render_style == Adaptive) {
        *max_width = text_width + (button->style.border_size * 2) + (button->style.padding * 2);
        *max_height = text_height + (button->style.border_size * 2) + (button->style.padding * 2);
    }
}


void register_button_callback(Button *button, void (*callback)())
{
    button->callback = callback;
}


void render_button(SDL_Renderer *renderer, Button *button)
{   
    if(button->showing) {
        SDL_Rect rect;
        
        // Real X and Y account for relative positioning inside of a container.
        int real_x = 0;
        int real_y = 0;
        if(button->container) {
            real_x = button->container->position.x + button->container->style.padding;
            real_y = button->container->position.y +
                get_drag_bar_position(button->container).h +
                button->container->style.padding;
        }

        // Set font before we start doing calculations on its size.
        set_font(button->style.font_key);

        int max_height, max_width;
        get_button_dimensions(button, &max_width, &max_height);

        // Border
        rect.x = real_x + button->position.x;
        rect.y = real_y + button->position.y;
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
        rect.x = real_x + button->position.x + button->style.border_size;
        rect.y = real_y + button->position.y + button->style.border_size;
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
        int x = real_x + button->position.x + button->style.border_size + button->style.padding;
        int y = real_y + button->position.y + button->style.border_size + button->style.padding;
        render_word(renderer, button->text, x, y);
    }
}