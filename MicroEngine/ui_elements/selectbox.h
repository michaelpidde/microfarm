#pragma once

#include "types.h"

SelectBox _create_selectbox(Rect position, char *id, int visible_elements);
void add_selectbox_element(SelectBox *sb, char *key, char *value, int selected);
void generate_options_image(SDL_Renderer *renderer, SelectBox *sb);