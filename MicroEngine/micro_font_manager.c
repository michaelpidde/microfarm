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


int index_of_int(int search, int *array, int array_size)
{
	for(int i = 0; i < array_size; i++) {
		if(array[i] == search) {
			return i;
		}
	}
	return -1;
}


Glyph get_glyph(char ch)
{
	int index = index_of_int((int)ch, _selected_font->id_link, GLYPH_COUNT);
    // TODO: This doesn't account for -1 return from index_of_int
	Glyph glyph = _selected_font->glyphs[index];
	return glyph;
}


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


int font_height()
{
	return *_selected_font->font_size + *_selected_font->leading;
}


int font_leading()
{
	return *_selected_font->leading;
}


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