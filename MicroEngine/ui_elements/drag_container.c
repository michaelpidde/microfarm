#include "drag_container.h"
#include "../cast.h"


ElementStyle get_default_container_style()
{
    ElementStyle cs;
    RGBColor base = {.r = 248, .g = 232, .b = 218};
    RGBColor border = {.r = 213, .g = 120, .b = 36};
    cs.base_color = base;
    cs.border_color = border;
    cs.border_size = 3;
    cs.padding = 7;
    return cs;
}


DragContainer _create_container(Rect position, char *id)
{
    DragContainer dc;
    dc.style = get_default_container_style();
    dc.position = position;
    dc.showing = 1;
    dc.dragbar_state = Off;
    strcpy(dc.id, id);
    return dc;
}


Rect get_drag_bar_position(DragContainer *container)
{
    Rect dimensions;
    dimensions.x = container->position.x + container->style.border_size;
    dimensions.y = container->position.y + container->style.border_size;
    dimensions.h = DRAG_BAR_HEIGHT;
    dimensions.w = container->position.w - (container->style.border_size * 2);
    return dimensions;
}


void render_container(SDL_Renderer *renderer, DragContainer *container)
{
    if(container->showing) {
        SDL_Rect rect = to_SDL_Rect(container->position);

        // Border
        SDL_SetRenderDrawColor(
            renderer,
            container->style.border_color.r,
            container->style.border_color.g,
            container->style.border_color.b,
            255
        );
        SDL_RenderFillRect(renderer, &rect);

        // Body
        rect.x += container->style.border_size;
        rect.y += container->style.border_size;
        rect.w -= (container->style.border_size * 2);
        rect.h -= (container->style.border_size * 2);
        SDL_SetRenderDrawColor(
            renderer,
            container->style.base_color.r,
            container->style.base_color.g,
            container->style.base_color.b,
            255
        );
        SDL_RenderFillRect(renderer, &rect);

        // Drag bar
        rect = to_SDL_Rect(get_drag_bar_position(container));
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