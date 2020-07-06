#include "micro_font_manager.h"
#include "fonts/cabin_20_black.h"

#define TOTAL_FONTS 10

typedef struct FontState {
    int *leading;
    int *font_size;
    Glyph *glyphs;
    int *id_link;
    SDL_Texture *glyph_texture;
} FontState;

typedef struct Fonts {
	FontState font_state[TOTAL_FONTS];
	char keys[TOTAL_FONTS][KEY_LENGTH];
	int ctr;
} Fonts;


Fonts _fonts;
FontState *_selected_font;


/*******************************************************************************
 * Initialize font writer to contain default engine fonts.
 * 
 * INPUT: none
 * 
 * OUTPUT: none
 ******************************************************************************/
void init_font(SDL_Renderer *renderer)
{
    load_asset_class(renderer, "res\\fonts", "res_font");

	// TODO: Make this not hard coded.
	FontState fs;
	fs.glyph_texture = get_asset("res_font_cabin_20_black");
	fs.glyphs = font_cabin_20_black_glyphs;
	fs.id_link = font_cabin_20_black_glyph_id_link;
	fs.leading = &font_cabin_20_leading;
	fs.font_size = &font_cabin_20_font_size;
	
	_fonts.font_state[_fonts.ctr] = fs;
	strcpy(_fonts.keys[_fonts.ctr], "res_font_cabin_20_black");
	++_fonts.ctr;
}


/*******************************************************************************
 * Takes a single word and renders it character by character.
 * 
 * INPUT:
 * SDL_Renderer * -- Renderer stored in engine state
 * char *         -- Word to render
 * int            -- X position on screen
 * int            -- Y position on screen
 * 
 * OUTPUT:
 * int            -- Width of the word that was rendered.
 ******************************************************************************/
int render_word(SDL_Renderer *renderer, char *word, int cursor_x, int cursor_y) {
	int len = strlen(word);
	int word_width = 0;
	int char_width = 0;
	for(int i = 0; i < len; ++i) {
		char_width = draw_character(renderer, word[i], cursor_x, cursor_y);
		word_width += char_width;
		cursor_x += char_width;
	}
	return word_width;
}


/*******************************************************************************
 * Tries to find index of integer in array of integers.
 * 
 * INPUT:
 * int   -- Integer to search for
 * int * -- Array to search in
 * int   -- Size of search array
 * 
 * OUTPUT:
 * int   -- Index of found integer. -1 if not found.
 ******************************************************************************/
int index_of_int(int search, int *array, int array_size)
{
	for(int i = 0; i < array_size; i++) {
		if(array[i] == search) {
			return i;
		}
	}
	return -1;
}


/*******************************************************************************
 * Gets structure containing information about a character in a glyphmap.
 * 
 * INPUT:
 * char  -- The character to get information about
 * 
 * OUTPUT:
 * Glyph -- Structure of character information
 ******************************************************************************/
Glyph get_glyph(char ch)
{
	int index = index_of_int((int)ch, _selected_font->id_link, GLYPH_COUNT);
    // TODO: This doesn't account for -1 return from index_of_int
	Glyph glyph = _selected_font->glyphs[index];
	return glyph;
}


/*******************************************************************************
 * 
 * 
 * INPUT:
 * SDL_Renderer * -- Renderer stored in engine state
 * char           -- Character to render
 * int            -- X position on screen
 * int            -- Y posiiton on screen
 * 
 * OUTPUT:
 * int            -- Xadvance to keep track of how much space was used.
 ******************************************************************************/
int draw_character(SDL_Renderer *renderer, char ch, int cursor_x, int cursor_y)
{
	Glyph glyph = get_glyph(ch);

	// Position of character in glyphmap
	SDL_Rect src = {
		.x = glyph.x,
		.y = glyph.y,
		.w = glyph.w,
		.h = glyph.h
	};

	// Position on screen
	SDL_Rect dest = {
		.x = cursor_x + glyph.xoffset,
		.y = cursor_y + glyph.yoffset,
		.w = glyph.w,
		.h = glyph.h
	};

	SDL_RenderCopy(renderer, _selected_font->glyph_texture, &src, &dest);

	return glyph.xadvance;
}


/*******************************************************************************
 * Gets total render width of word based on glyph sizes.
 * 
 * INPUT:
 * char * -- Word to get render width of
 * 
 * OUTPUT:
 * int    -- Render width
 ******************************************************************************/
int get_word_width(char *word)
{
	int len = strlen(word);
	int width = 0;
	Glyph glyph;
	for(int i = 0; i < len; ++i) {
		glyph = get_glyph(word[i]);
		width += glyph.xadvance;
	}
	return width;
}


/*******************************************************************************
 * Set reference to font for subsequent render operations.
 * 
 * INPUT:
 * char * -- Unique name of font
 * 
 * OUTPUT: none
 ******************************************************************************/
void set_font(char *font)
{
	int index = 0;
	for(int i = 0; i < _fonts.ctr; ++i) {
		if(strcmp(_fonts.keys[i], font) == 0) {
			index = i;
		}
	}

	_selected_font = &_fonts.font_state[index];
}