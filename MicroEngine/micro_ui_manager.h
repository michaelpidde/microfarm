#pragma once
#include "constants.h"

typedef struct RGBColor {
    unsigned int r;
    unsigned int g;
    unsigned int b;
} RGBColor;

#define DEFAULT_UI_ELEMENT_FONT "font_cabin_20_black"
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

/*******************************************************************************
 * Off: State when mouse is not touching element.
 * 
 * Hover: State when mouse is over element.
 * 
 * Click: State when mouse is over element and mouse button is clicked.
 ******************************************************************************/
typedef enum ElementState {
    Off, Hover, Click
} ElementState;

/*******************************************************************************
 * Adaptive: Uses element width and height as starting values but will expand to
 * accommodate inner content size.
 * 
 * Strict: Uses element width and height verbatim and will clip inner content to
 * maintain static dimensions.
 ******************************************************************************/
typedef enum RenderStyle {
    Adaptive, Strict
} RenderStyle;

#define MAX_BUTTON_TEXT 50
typedef struct Button {
    char text[MAX_BUTTON_TEXT];
    uint32 x;
    uint32 y;
    uint32 w;
    uint32 h;
    char id[KEY_LENGTH];
    ButtonStyle style;
    ElementState state;
} Button;

#define MAX_BUTTONS 20
typedef struct UI_State {
    Button buttons[MAX_BUTTONS];
    uint32 button_ctr;
    RenderStyle render_style;
} UI_State;