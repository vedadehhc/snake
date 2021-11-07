#ifndef SCREEN_H
#define SCREEN_H

// warning! won't compile without makefile
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

class Screen {
private:
	// screen dimensions
	int width, height;
	
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;
	
	// high score
	int highscore = 0;
	
public:
	Screen(int screenWidth, int screenHeight) {
		width = screenWidth;
		height = screenHeight;
		
		
	}
	
	bool init() {
		bool success = true;
		
		//Initialize SDL
		if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
		{
			printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create window
			gWindow = SDL_CreateWindow( "Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
			if( gWindow == NULL )
			{
				printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
				if (gRenderer == NULL) {
					printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
					success = false;
				} else {			
					//Initialize renderer color
					SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
					SDL_RenderClear( gRenderer );
					
					// draw loading symbols
					SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					SDL_Rect one = {width / 2 - 20, height / 2, 10, 10};
					SDL_Rect two = {width / 2, height / 2, 10, 10};
					SDL_Rect three = {width / 2 + 20, height / 2, 10, 10};
					SDL_RenderFillRect( gRenderer, &one );
					SDL_RenderFillRect( gRenderer, &two );
					SDL_RenderFillRect( gRenderer, &three );
					
					// Update the renderer
					SDL_RenderPresent( gRenderer );
					
					
					// Initialize image loading
					int imgFlags = IMG_INIT_PNG;
					if ( !(IMG_Init(imgFlags) & imgFlags) ) {
						printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
						success = false;
					}
					
					// initialize font
					if (TTF_Init() == -1) {
						printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
						success = false;
					}
					
					// initialize sound
					 if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
					{
						printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
						success = false;
					}
					
					// load high score
					FILE* datafile;
					datafile = fopen("data.txt", "r");
					if (datafile != NULL) {					
						fscanf(datafile, "%d", &highscore);
						fclose(datafile);
					}
				}	
			}
		}
		
		return success;
	}

	
	// getters
	int getWidth() {
		return width;
	}
	
	int getHeight() {
		return height;
	}
};

#endif