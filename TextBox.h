#ifndef TEXTBOX_h
#define TEXTBOX_h

#include <string>
#include <SDL_ttf.h>
#include <SDL.h>


class TextBox {
private:
	int width, height;
	std::string text;
	
	// texture used to render on screen
	SDL_Texture* gTextTexture = NULL;
	
	// text color
	SDL_Color color = { 0xFF, 0, 0, 0xFF };
	
	// font
	TTF_Font* gFont = NULL;
	
public:
	TextBox(std::string s) : text(s) {
		
	}
	
	bool free() {		
		if ( gTextTexture != NULL ) {
			SDL_DestroyTexture(gTextTexture);
			gTextTexture = NULL;
			width = 0;
			height = 0;
		}
	}
	
	
	void setText(std::string s) {
		text = s;
		SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped( gFont, text.c_str(), color, 200 );
	}
	
	std::string getText() {
		return text;
	}
	
}

#endif