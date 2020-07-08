#include "drag_container.h"


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


DragContainer _create_container(uint32 x, uint32 y, uint32 w, uint32 h, char *id)
{
    DragContainer dc;
    dc.style = get_default_container_style();
    dc.x = x;
    dc.y = y;
    dc.w = w;
    dc.h = h;
    dc.showing = 1;
    dc.dragbar_state = Off;
    strcpy(dc.id, id);
    return dc;
}


Rect get_drag_bar_position(DragContainer *container)
{
    Rect dimensions;
    dimensions.x = container->x + container->style.border_size;
    dimensions.y = container->y + container->style.border_size;
    dimensions.h = DRAG_BAR_HEIGHT;
    dimensions.w = container->w - (container->style.border_size * 2);
    return dimensions;
}


void render_container(SDL_Renderer *renderer, DragContainer *container)
{
    if(container->showing) {
        SDL_Rect rect;

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