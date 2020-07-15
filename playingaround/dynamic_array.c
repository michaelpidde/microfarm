#include "stdio.h"
#include "stdlib.h"

#define uint32 unsigned int

typedef struct Rect {
    uint32 x;
    uint32 y;
    uint32 w;
    uint32 h;
} Rect;

typedef struct MyThing {
    Rect position;
    char *name;
} MyThing;

void push_thing(MyThing *stack, int *stack_ctr, Rect rect, char *name)
{
    ++*stack_ctr;
    realloc(stack, *stack_ctr * sizeof(MyThing));
    stack[*stack_ctr - 1].name = name;
    stack[*stack_ctr - 1].position = rect;
}

void main()
{
    Rect pos;
    pos.x = 0;
    pos.y = 0;
    pos.w = 100;
    pos.h = 100;

    MyThing *things = malloc(1 * sizeof(MyThing));
    int things_ctr = 0;
    push_thing(things, &things_ctr, pos, "Michael");

    pos.x += 99;
    pos.y += 99;
    push_thing(things, &things_ctr, pos, "Liz");

    printf("Name: %s\n", things[0].name);
    printf("X: %d, Y: %d, W: %d, H: %d\n", things[0].position.x, things[0].position.y, things[0].position.w, things[0].position.h);
    printf("Name: %s\n", things[1].name);
    printf("X: %d, Y: %d, W: %d, H: %d\n", things[1].position.x, things[1].position.y, things[1].position.w, things[1].position.h);
}