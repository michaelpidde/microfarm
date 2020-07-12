#include "selectbox.h"
#include "drag_container.h"


ElementStyle get_default_selectbox_style()
{
    ElementStyle cs;
    RGBColor base = {.r = 248, .g = 232, .b = 218};
    RGBColor border = {.r = 213, .g = 120, .b = 36};
    cs.base_color = base;
    cs.border_color = border;
    cs.border_size = 1;
    cs.padding = 3;
    return cs;
}


SelectBox _create_selectbox(Rect position, char *id, int visible_elements)
{
    SelectBox sb;
    strcpy(sb.id, id);
    sb.position = position;
    sb.element_selected = 0;
    sb.showing = 1;
    sb.scroll_amount = 0;
    sb.visible_elements = visible_elements;
    sb.style = get_default_selectbox_style();
    sb.callback = NULL;
    return sb;
}


void add_selectbox_element(SelectBox *sb, char *key, char *value, int selected)
{
    if(sb->element_ctr < MAX_SELECT_OPTIONS) {
        strcpy(sb->keys[sb->element_ctr], key);
        strcpy(sb->values[sb->element_ctr], value);
        if(selected) {
            sb->element_selected = sb->element_ctr;
        }
        ++sb->element_ctr;
    } else {
        error("Max number of options added to select box.\n");
    }
}


void _get_selectbox_dimensions(RenderStyle render_style, SelectBox *sb, int *max_width, int *max_height)
{
    // TODO: This is really stupid and I shouldn't have to do this here.
    set_font(sb->style.font_key);

    int text_height = font_height();

    *max_width = sb->position.w;

    *max_height = sb->position.h + (sb->style.border_size * 2);
    if(render_style == Adaptive) {
        *max_height = (text_height * sb->visible_elements) + (sb->style.border_size * 2) + (sb->style.padding * 2);
    }
}


void generate_options_image(SDL_Renderer *renderer, SelectBox *sb)
{
    // Set font before we start doing calculations on its size.
    set_font(sb->style.font_key);

    int w = sb->position.w;
    int h = sb->element_ctr * font_height();
    SDL_Texture *texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(renderer, texture);

    // Clear to element base color.
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = w;
    src.h = h;
    SDL_SetRenderDrawColor(renderer, sb->style.base_color.r, sb->style.base_color.g, sb->style.base_color.b, 255);
    SDL_RenderFillRect(renderer, &src);

    SDL_Rect dest;
    int x = 0;
    int y = 0;
    for(int i = 0; i < sb->element_ctr; ++i) {
        render_word(renderer, sb->values[i], x, y);
        y += font_height();
    }
    sb->options_texture = texture;
    sb->options_texture_h = h;
    sb->options_texture_w = w;

    // Reset renderer to default target.
    SDL_SetRenderTarget(renderer, NULL);
}


void render_selectbox(SDL_Renderer *renderer, SelectBox *sb)
{
    if(sb->showing) {
        SDL_Rect rect;

        // Real X and Y account for relative positioning inside of a container.
        int real_x = 0;
        int real_y = 0;
        if(sb->container) {
            real_x = sb->container->position.x + sb->container->style.padding;
            real_y = sb->container->position.y +
                get_drag_bar_position(sb->container).h +
                sb->container->style.padding;
        }

        // Set font before we start doing calculations on its size.
        set_font(sb->style.font_key);

        int max_height, max_width;
        get_selectbox_dimensions(sb, &max_width, &max_height);

        // Border
        rect.x = real_x + sb->position.x;
        rect.y = real_y + sb->position.y;
        rect.w = max_width;
        rect.h = max_height;
        SDL_SetRenderDrawColor(
            renderer,
            sb->style.border_color.r,
            sb->style.border_color.g,
            sb->style.border_color.b,
            255
        );
        SDL_RenderFillRect(renderer, &rect);

        // Body
        rect.x = real_x + sb->position.x + sb->style.border_size;
        rect.y = real_y + sb->position.y + sb->style.border_size;
        rect.w = max_width - (sb->style.border_size * 2);
        rect.h = max_height - (sb->style.border_size * 2);
        RGBColor *state_color = &sb->style.base_color;
        SDL_SetRenderDrawColor(
            renderer,
            state_color->r,
            state_color->g,
            state_color->b,
            255
        );
        SDL_RenderFillRect(renderer, &rect);

        // Text
        rect.x += sb->style.padding;
        rect.y += sb->style.padding;
        rect.w -= sb->style.padding * 2;
        rect.h -= sb->style.padding * 2;

        SDL_Rect src;
        src.x = 0;
        src.y = sb->scroll_amount;
        src.w = rect.w;
        if(sb->element_ctr < sb->visible_elements) {
            // Constrain the source and destination rectangle height so we don't get image distortion.
            src.h = sb->element_ctr * font_height();
            rect.h = src.h;
        } else {
            src.h = rect.h;
        }
        SDL_RenderCopy(renderer, sb->options_texture, &src, &rect);
    }
}