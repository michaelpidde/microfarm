#pragma once

typedef enum {
    North, East, South, West
} Direction;

#define PLAYER_H 64
#define PLAYER_W 32
typedef struct Player {
    Point position;
    char *asset_key;
    int speed;
    float velocity;
} Player;

typedef struct KeyState {
    int w;
    int a;
    int s;
    int d;
} KeyState;

typedef struct State {
    Player player;
    KeyState keys_down;
} State;