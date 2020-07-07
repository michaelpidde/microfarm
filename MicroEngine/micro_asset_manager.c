#include "micro_engine.h"
#include "micro_asset_manager.h"

static Assets _assets;


/*******************************************************************************
 * Module initialization.
 * 
 * INPUT:
 * SDL_Renderer * -- Renderer stored in engine state
 * 
 * OUTPUT: none
 ******************************************************************************/
void init_asset_manager(SDL_Renderer *renderer)
{
    _assets.renderer = renderer;
    _assets.ctr = 0;
}


/*******************************************************************************
 * Find character in string.
 * 
 * INPUT:
 * char   -- The character to search for
 * char * -- The string to search in
 * 
 * OUTPUT:
 * int    -- The index of needle, if found, otherwise -1
 ******************************************************************************/
int str_find(char needle, char *haystack) {
    char * find = strrchr(haystack, needle);
    if(find != NULL) {
        return find - haystack;
    }
    return -1;
}


/*******************************************************************************
 * Get texture from loaded assets dictionary by unique name. This function will
 * return a dud texture in the event that the requested asset is not found. This
 * will visually indicate that there is a missing asset.
 * 
 * INPUT:
 * char *      -- unique name of asset to look for
 * 
 * OUTPUT:
 * SDL_Texture -- Asset texture
 ******************************************************************************/
SDL_Texture *get_asset(char *key) {
    int pos = -1;
    for(int i = 0; i < _assets.ctr; ++i) {
        if(strcmp(key, _assets.index[i]) == 0) {
            pos = i;
        }
    }

    if(pos == -1) {
        char buffer[50];
        sprintf(buffer, "Couldn't find texture %s\n", key);
        error(buffer);
        // TODO: Do something about hard coded width and height. Also, using global without passing it in...
        SDL_Texture *dud = SDL_CreateTexture(_assets.renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 100, 100);
        SDL_SetRenderTarget(_assets.renderer, dud);
        SDL_SetRenderDrawColor(_assets.renderer, 255, 0, 255, 255);
        SDL_Rect dud_rect;
        dud_rect.x = 0;
        dud_rect.y = 0;
        dud_rect.w = 100;
        dud_rect.h = 100;
        SDL_RenderFillRect(_assets.renderer, &dud_rect);
        SDL_SetRenderTarget(_assets.renderer, NULL);
        return dud;
    } else {
        return _assets.textures[pos];
    }
}


/*******************************************************************************
 * 
 * 
 * INPUT:
 * SDL_Renderer -- Renderer stored in engine state
 * char *       -- Directory path to load assets from
 * char *       -- Prefix for unique keys to reference assets by
 * 
 * OUTPUT:
 * int          -- Number of assets loaded
 ******************************************************************************/
int load_asset_class(SDL_Renderer *renderer, char *dir, char *prefix)
{
    char *files[MAX_DIR_FILES];
    int file_ctr = win32_get_directory_files(files, dir, "*.png");
    int textures_loaded = 0;

    if(file_ctr < 0) {
        char buffer[50];
        sprintf(buffer, "Error loading asset class %s\n", prefix);
        error(buffer);
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
                char buffer[100];
                sprintf(buffer, "Failed to load texture %s. Error: %s\n", full_path, IMG_GetError());
                error(buffer);
            } else {
                _assets.textures[_assets.ctr] = SDL_CreateTextureFromSurface(renderer, loading_surface);
                int ext_index = str_find('.', files[i]);
                char *dest = _assets.index[_assets.ctr];
                strcat(dest, prefix);
                strcat(dest, "_");
                strncat(dest, files[i], ext_index);
                // printf("Loaded %s: %s\n", dest, full_path);
                ++_assets.ctr;
                ++textures_loaded;
                SDL_FreeSurface(loading_surface);
            }
        } else {
            // TODO: Do something better here
            error("Loaded max assets");
        }
    }

    win32_free_directory_list(files, file_ctr);

    #if DEBUG
        char buffer[100];
        sprintf(buffer, "Loaded %d texture(s) from %s", textures_loaded, dir);
        debug_string(buffer, Green, Gray, 0);
    #endif

    return textures_loaded;
}


void free_assets()
{
    // TODO: Implement this. This only happens at the end of the game, so maybe ignore it and let the system do it.
}