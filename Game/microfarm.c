#include "microfarm.h"

int main()
{
    ME_init();
    int loaded = ME_load_asset_class("D:\\Code\\microfarm\\build\\assets\\player", "player");
    printf("Assets loaded: %d\n", loaded);

    // Start game loop
}