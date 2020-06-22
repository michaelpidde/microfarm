#include "micro_engine.h"
#include "micro_asset_manager.h"

static Assets _assets;

void init_asset_manager()
{
    _assets.ctr = 0;
}

int str_find(char needle, char *haystack) {
    char * find = strrchr(haystack, needle);
    if(find != NULL) {
        return find - haystack;
    }
    return -1;
}

int load_asset_class(SDL_Renderer *renderer, char *dir, char *prefix)
{
    char *files[MAX_DIR_FILES];
    int file_ctr = win32_get_directory_files(files, dir, "*.png");
    int textures_loaded = 0;

    if(file_ctr < 0) {
        printf("Error loading asset class %s\n", prefix);
        return 0;
    }

    for(int i = 0; i < file_ctr; ++i) {
        if(_assets.ctr < MAX_ASSETS) {
            SDL_Texture *texture = NULL;
            SDL_Surface *loading_surface;
            char full_path[MAX_PATH_LENGTH];
            sprintf(full_path, "%s\\%s", dir, files[i]);
            loading_surface = IMG_Load(full_path);

            if(loading_surface == NULL) {
                printf("Failed to load texture %s. Error: %s\n", full_path, IMG_GetError());
            } else {
                _assets.textures[_assets.ctr] = SDL_CreateTextureFromSurface(renderer, loading_surface);
                int ext_index = str_find('.', files[i]);
                strncpy(_assets.index[_assets.ctr], files[i], ext_index);
                ++_assets.ctr;
                ++textures_loaded;
                SDL_FreeSurface(loading_surface);
            }
        } else {
            // TODO: Do something better here
            printf("Loaded max assets\n");
        }
    }

    win32_free_directory_list(files, file_ctr);

    return textures_loaded;
}

void free_assets()
{
    // TODO: Implement this. This only happens at the end of the game, so maybe ignore it and let the system do it.
}