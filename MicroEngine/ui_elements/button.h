#pragma once

#include "types.h"

Button _create_button(uint32 x, uint32 y, uint32 w, uint32 h, char *id, char *text);
void _get_button_dimensions(RenderStyle render_style, Button *button, int *max_width, int *max_height);
void register_button_callback(Button *button, void (*callback)());