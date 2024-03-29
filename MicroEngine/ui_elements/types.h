#pragma once

#include "constants.h"
#include "../../Common/types.h"
#include <SDL.h>

typedef enum ElementState {
    Off, Hover, Click, MouseWheel
} ElementState;

typedef struct ElementStyle {
    RGBColor base_color;
    RGBColor border_color;
    uint32 border_size;
    uint32 padding;
    char font_key[KEY_LENGTH];
} ElementStyle;

typedef struct DragContainer {
    Rect position;
    ElementStyle style;
    ElementState dragbar_state;
    char id[KEY_LENGTH];
    int showing;
    int close_button;
    int dragging;
    void (*close_callback)();
} DragContainer;

typedef struct SelectBox {
    Rect position;
    ElementStyle style;
    ElementState state;
    char keys[MAX_SELECT_OPTIONS][KEY_LENGTH];
    char values[MAX_SELECT_OPTIONS][KEY_LENGTH];
    int element_ctr;
    int element_selected;
    int visible_elements;
    char id[KEY_LENGTH];
    DragContainer *container;
    SDL_Texture *options_texture;
    int options_texture_w;
    int options_texture_h;
    int showing;
    int doing_callback;
    int scroll_amount;
    void (*callback)();
} SelectBox;

typedef struct TileGrid {
    Rect position;
    
} TileGrid;

/**
 * Adaptive: Uses element width and height as starting values but will expand to
 * accommodate inner content size.
 * 
 * Strict: Uses element width and height verbatim and will clip inner content to
 * maintain static dimensions.
 */
typedef enum RenderStyle {
    Adaptive, Strict
} RenderStyle;

typedef struct ButtonStyle {
    RGBColor base_color;
    RGBColor hover_color;
    RGBColor click_color;
    RGBColor border_color;
    uint32 border_size;
    uint32 padding;
    int text_alignment;
    char font_key[KEY_LENGTH];
} ButtonStyle;

typedef struct Button {
    char text[MAX_BUTTON_TEXT];
    Rect position;
    char id[KEY_LENGTH];
    ButtonStyle style;
    ElementState state;
    DragContainer *container;
    int showing;
    int doing_callback;
    void (*callback)();
} Button;