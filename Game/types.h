#pragma once

typedef enum {
    North, East, South, West
} Direction;

typedef struct Player {
    Point position;
    char *asset_key;
    int speed;
    float velocity;
} Player;

typedef struct State {
    Player player;
} State;