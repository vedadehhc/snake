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
	
	// Roboto font
	TTF_Font* gFont = NULL;
	
public:
	TextBox(std::string s) : text(s) {
		
	}
	
	bool free() {
		
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