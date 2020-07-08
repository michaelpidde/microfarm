#pragma once

#include "constants.h"
#include "../../Common/types.h"

typedef enum ElementState {
    Off, Hover, Click
} ElementState;

typedef struct ContainerStyle {
    RGBColor base_color;
    RGBColor border_color;
    uint32 border_size;
    uint32 padding;
} ContainerStyle;

typedef struct DragContainer {
    uint32 x;
    uint32 y;
    uint32 w;
    uint32 h;
    ContainerStyle style;
    ElementState dragbar_state;
    char id[KEY_LENGTH];
    int showing;
} DragContainer;

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
    uint32 x;
    uint32 y;
    uint32 w;
    uint32 h;
    char id[KEY_LENGTH];
    ButtonStyle style;
    ElementState state;
    DragContainer *container;
    int showing;
    int doing_callback;
    void (*callback)();
} Button;