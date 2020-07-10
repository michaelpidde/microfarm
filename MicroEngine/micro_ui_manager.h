#pragma once
#include "ui_elements/types.h"
#include "constants.h"
#include "ui_elements/button.h"
#include "ui_elements/drag_container.h"
#include "ui_elements/selectbox.h"

typedef struct UI_State {
    Button buttons[MAX_BUTTONS];
    uint32 button_ctr;
    DragContainer containers[MAX_CONTAINERS];
    uint32 container_ctr;
    SelectBox selectboxes[MAX_SELECT_ELEMENTS];
    uint32 select_ctr;
    RenderStyle render_style;
    uint32 last_mouse_x;
    uint32 last_mouse_y;
} UI_State;

Button *create_button(Rect position, char *id, char *text);
void get_button_dimensions(Button *button, int *max_width, int *max_height);
Button *get_button(char *id);
DragContainer *create_container(Rect position, char *id);
DragContainer *get_container(char *id);
SelectBox *create_selectbox(Rect position, char *id, int visible_elements);
SelectBox *get_selectbox(char *id);
void get_selectbox_dimensions(SelectBox *sb, int *max_width, int *max_height);