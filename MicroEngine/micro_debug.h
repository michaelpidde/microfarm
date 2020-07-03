#define DEBUG_STYLE_BOLD (1 << 0)
#define DEBUG_STYLE_UNDERLINE (1 << 1)
#define DEBUG_STYLE_NEGATIVE (1 << 2)

typedef enum Color {
    Red, LightRed,
    Green, LightGreen,
    Yellow, LightYellow,
    Blue, LightBlue,
    Magenta, LightMagenta,
    Cyan, LightCyan,
    Gray, LightGray,
    Black, White
} Color;

void error(char *buffer);
void debug_string_simple(char *buffer, Color foreground);
void debug_string(char *buffer, Color foreground, Color background, int style);