#pragma once

#define uint32 unsigned int

typedef struct Point {
    float x;
    float y;
} Point;

typedef struct Rect {
    int x;
    int y;
    int w;
    int h;
} Rect;

typedef struct RGBColor {
    unsigned int r;
    unsigned int g;
    unsigned int b;
} RGBColor;