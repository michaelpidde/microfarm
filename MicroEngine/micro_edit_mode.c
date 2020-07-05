#include "micro_engine.h"

void init_editor()
{
    int x = 300;
    int y = 300;
    int width, height;
    Button *button = create_button(x, y, 0, 0, "btn_test", "Test Button");
    get_button_dimensions(button, &width, &height);
    x += width + 10;
    button = create_button(x, y, 0, 0, "btn_test2", "Another Button");
    // register_button_callback(button, &stupid_callback);
    get_button_dimensions(button, &width, &height);
    x += width + 10;
    create_button(x, y, 0, 0, "btn_test3", "Beep");
}

void toggle_editor(int showing)
{
    get_button_by_id("btn_test")->showing = showing;
    get_button_by_id("btn_test2")->showing = showing;
    get_button_by_id("btn_test3")->showing = showing;
}