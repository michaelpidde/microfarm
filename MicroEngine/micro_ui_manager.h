#pragma once
#include "constants.h"

#define DEFAULT_UI_ELEMENT_FONT "e_font_cabin_20_black"
#define MAX_BUTTON_TEXT 50
// TODO: Make this dynamic somehow.
#define MAX_BUTTONS 20
#define MAX_CONTAINERS 20
#define DRAG_BAR_HEIGHT 23

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
    uint32 border_size;
    uint32 padding;
    int text_alignment;
    char font_key[KEY_LENGTH];
} ButtonStyle;

typedef struct ContainerStyle {
    RGBColor base_color;
    RGBColor border_color;
    uint32 border_size;
    uint32 padding;
} ContainerStyle;

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

typedef struct UI_State {
    Button buttons[MAX_BUTTONS];
    uint32 button_ctr;
    DragContainer containers[MAX_CONTAINERS];
    uint32 container_ctr;
    RenderStyle render_style;
    uint32 last_mouse_x;
    uint32 last_mouse_y;
} UI_State;

Button *create_button(uint32 x, uint32 y, uint32 w, uint32 h, char *id, char *text);
Button *get_button_by_id(char *id);
void get_button_dimensions(Button *button, int *max_width, int *max_height);
void register_button_callback(Button *button, void (*callback)());
DragContainer *create_container(uint32 x, uint32 y, uint32 w, uint32 h, char *id);
DragContainer *get_container_by_id(char *id);