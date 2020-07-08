#include "micro_engine.h"
#include "micro_debug.h"


void error(char *buffer)
{
    debug_string(buffer, White, Red, DEBUG_STYLE_BOLD);
}


void console_foreground_color(Color color)
{
    switch(color) {
        case Red: {
            printf("\033[31m");
        } break;
        case LightRed: {
            printf("\033[91m");
        }
        case Green: {
            printf("\033[32m");
        } break;
        case LightGreen: {
            printf("\033[92m");
        } break;
        case Yellow: {
            printf("\033[33m");
        } break;
        case LightYellow: {
            printf("\033[93m");
        } break;
        case Blue: {
            printf("\033[34m");
        } break;
        case LightBlue: {
            printf("\033[94m");
        } break;
        case Magenta: {
            printf("\033[35m");
        } break;
        case LightMagenta: {
            printf("\033[95m");
        } break;
        case Cyan: {
            printf("\033[36m");
        } break;
        case LightCyan: {
            printf("\033[96m");
        } break;
        case Gray: {
            printf("\033[90m");
        } break;
        case LightGray: {
            printf("\033[37m");
        } break;
        case Black: {
            printf("\033[30m");
        } break;
        case White: {
            printf("\033[97m");
        } break;
    }
}


void console_background_color(Color color)
{
    switch(color) {
        case Red: {
            printf("\033[41m");
        } break;
        case LightRed: {
            printf("\033[101m");
        }
        case Green: {
            printf("\033[42m");
        } break;
        case LightGreen: {
            printf("\033[102m");
        } break;
        case Yellow: {
            printf("\033[43m");
        } break;
        case LightYellow: {
            printf("\033[103m");
        } break;
        case Blue: {
            printf("\033[44m");
        } break;
        case LightBlue: {
            printf("\033[104m");
        } break;
        case Magenta: {
            printf("\033[45m");
        } break;
        case LightMagenta: {
            printf("\033[105m");
        } break;
        case Cyan: {
            printf("\033[46m");
        } break;
        case LightCyan: {
            printf("\033[106m");
        } break;
        case Gray: {
            printf("\033[100m");
        } break;
        case LightGray: {
            printf("\033[47m");
        } break;
        case Black: {
            printf("\033[40m");
        } break;
        case White: {
            printf("\033[107m");
        } break;
    }
}


void debug_string_simple(char *buffer, Color foreground)
{
    console_foreground_color(foreground);
    printf(buffer);
    // Reset
    printf("\033[0m");
    printf("\n");
}


void debug_string(char *buffer, Color foreground, Color background, int style)
{
    console_foreground_color(foreground);
    console_background_color(background);

    if(style & DEBUG_STYLE_BOLD) {
        printf("\033[1m");
    }
    if(style & DEBUG_STYLE_UNDERLINE) {
        printf("\033[4m");
    }
    if(style & DEBUG_STYLE_NEGATIVE) {
        printf("\033[7m");
    }

    printf(buffer);
    // Reset
    printf("\033[0m");
    printf("\n");
}