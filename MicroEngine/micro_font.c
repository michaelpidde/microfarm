#include "micro_engine.h"
#include "SDL_ttf.h"

#define TOTAL_FONTS 10
#define FONT_PATH_LENGTH 100

typedef struct Fonts {
    char paths[TOTAL_FONTS][FONT_PATH_LENGTH];
    char keys[TOTAL_FONTS][ASSET_KEY_LENGTH];
    int ctr;
} Fonts;


Fonts _fonts;


/*******************************************************************************
 * Font library initialization.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 ******************************************************************************/
void init_font()
{
    if(TTF_Init() == -1) {
        printf("Failed to initialize TTF support.\n");
        printf("Error: %s", TTF_GetError());
    }
}


/*******************************************************************************
 * Load font path identified by unique key.
 * 
 * INPUT:
 * char * -- Path to font file
 * char * -- Unique key
 * 
 * OUTPUT: none
 ******************************************************************************/
void load_font(char *path, char *key)
{
    if(_fonts.ctr < TOTAL_FONTS) {
        strcpy(_fonts.paths[_fonts.ctr], path);
        strcpy(_fonts.keys[_fonts.ctr], key);
        ++_fonts.ctr;
    } else {
        printf("Max fonts already loaded.\n");
    }
}

char *get_font_path(char *key)
{
    char *path = "";
    for(int i = 0; i < TOTAL_FONTS; ++i) {
        if(strcmp(_fonts.keys[i], key) == 0) {
            return _fonts.paths[i];
        }
    }
    return path;
}

SDL_Texture *get_text_texture(SDL_Renderer *renderer, char *key, char *text, int size)
{
    TTF_Font *font = TTF_OpenFont(get_font_path(key), size);
    if(!font) {
        printf("Text render error: %s\n", TTF_GetError());
    }
    SDL_Color color = {0, 0, 0};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
    return texture;
}