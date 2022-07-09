//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <queue>

// TESTING:
#include "Screen.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTextTexture = NULL;
int textWidth;
int textHeight;

// Roboto font
TTF_Font* gFont = NULL;

// Sound effects
Mix_Chunk* gCollect = NULL;
Mix_Chunk* gDeath = NULL;

bool gSoundEnabled = true;

// high score
int highscore = 0;

enum GameStates {
	LOADING,
	MENU,
	PLAY,
	DEAD
};

int gameState = LOADING;


// Starts up SDL and creates window
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
		gWindow = SDL_CreateWindow( "Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
				
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_Rect one = {SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT / 2, 10, 10};
				SDL_Rect two = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 10, 10};
				SDL_Rect three = {SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2, 10, 10};
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


// Frees media and closes SDL
bool close() {
	
	FILE* datafile;
	datafile = fopen("data.txt", "w");
	fprintf(datafile, "%d", highscore);
	fclose(datafile);
	
	// free sfx
	Mix_FreeChunk(gCollect);
	Mix_FreeChunk(gDeath);
	gCollect = NULL;
	gDeath = NULL;
	
	// Deallocate text
	SDL_DestroyTexture(gTextTexture);
	gTextTexture = NULL;
	
	// free font
	TTF_CloseFont(gFont);
	gFont = NULL;
	
	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow( gWindow );
	gRenderer = NULL;
	gWindow = NULL;
	

	//Quit SDL subsystems
	Mix_Quit();	
	IMG_Quit();
	SDL_Quit();
}

/*
SDL_Texture* loadTexture( std::string path ) {
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}
*/


void freeText() {
	if ( gTextTexture != NULL ) {
		SDL_DestroyTexture(gTextTexture);
		gTextTexture = NULL;
		textWidth = 0;
		textHeight = 0;
	}
}

bool updateText(std::string text) {
	freeText();
	SDL_Color color = { 0xFF, 0, 0, 0xFF };
	
	//Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped( gFont, text.c_str(), color, 200 );
	if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        gTextTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( gTextTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            textWidth = textSurface->w;
            textHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    //Return success
    return gTextTexture != NULL;
}

bool loadMedia() {
	bool success = true;
	
	// font
	gFont = TTF_OpenFont("fonts/RobotoMono.ttf", 24);
    if( gFont == NULL )
    {
        printf( "Failed to load Roboto font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Render text
        if( !updateText( "Score: 0\nHighscore: 0" ) )
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
    }
	
	// window icon
	SDL_Surface* iconSurface = IMG_Load("images/icon.png");
	if (iconSurface == NULL) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", "images/icon.png", IMG_GetError() );
		success = false;
	} else {
		SDL_SetWindowIcon(gWindow, iconSurface);
		SDL_FreeSurface(iconSurface);
	}
	
	// sound fx
	gCollect = Mix_LoadWAV( "sounds/collect.wav" );
	gDeath = Mix_LoadWAV("sounds/death.wav");
	if (gCollect == NULL || gDeath == NULL) {
        printf( "Failed to load collect sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
	}
	
	return success;
}



void drawGrid(int spacing) {
	for(int i = 0; i < SCREEN_WIDTH; i += spacing) {
		SDL_RenderDrawLine( gRenderer, i, 0, i, SCREEN_HEIGHT );
	}
	for(int i = 0; i < SCREEN_HEIGHT; i += spacing) {
		SDL_RenderDrawLine( gRenderer, 0, i, SCREEN_WIDTH, i );
	}
}


enum Direction {
	EAST,
	SOUTH,
	NORTH,
	WEST,
	TOTAL
};


int curX = SCREEN_WIDTH / 2;
int curY = SCREEN_HEIGHT / 2;
int curDir = EAST;
std::queue<int> moves;
std::queue<int> bodyX;
std::queue<int> bodyY;
int score = 0;

const int PLAYER_SIZE = 40;
const int PLAYER_SPEED = PLAYER_SIZE;
int DELAY = 200;


enum Cell {
	EMPTY,
	HEAD,
	BODY,
	FOOD
};

const int GRID_WIDTH = SCREEN_WIDTH / PLAYER_SIZE;
const int GRID_HEIGHT = SCREEN_HEIGHT / PLAYER_SIZE;
const int GRID_SIZE = PLAYER_SIZE;

int grid[GRID_WIDTH][GRID_HEIGHT] = {};




bool turn() {
	bool moved = false;
	if(!moves.empty()) {
		int move = moves.front();
		moves.pop();
		
		if (move + curDir != TOTAL - 1) {
			curDir = move;
		}
		
		moved = true;
	}
	return moved;
}

void render() {
	
	// clear the screen
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
	SDL_RenderClear( gRenderer );
	
	if (gameState == MENU) {
		// render text
		SDL_Rect textQuad = { SCREEN_WIDTH / 2 - textWidth / 2,  SCREEN_HEIGHT / 2 - textHeight / 2, textWidth, textHeight};
		SDL_RenderCopy( gRenderer, gTextTexture, NULL, &textQuad );
	} else if (gameState == PLAY || gameState == DEAD) {	
		// Render cells
		for(int i = 0; i < GRID_WIDTH; i++) {
			for (int j = 0; j < GRID_HEIGHT; j++) {
				if (grid[i][j] == FOOD) {
					SDL_Rect foodRect = { i * GRID_SIZE + 3, j * GRID_SIZE + 3, GRID_SIZE - 6, GRID_SIZE - 6};
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );        
					SDL_RenderFillRect( gRenderer, &foodRect );
				} else if (grid[i][j] == BODY) {
					SDL_Rect bodyRect = { i * GRID_SIZE, j * GRID_SIZE, GRID_SIZE - 1, GRID_SIZE - 1};
					if (gameState == PLAY) {
						SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					} else if (gameState == DEAD) {
						SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
					}
					SDL_RenderFillRect( gRenderer, &bodyRect );
				}
			}
		}
		
		// Render player head
		SDL_Rect fillRect = { curX, curY, PLAYER_SIZE - 1, PLAYER_SIZE - 1};
		if (gameState == PLAY) {
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		} else if (gameState == DEAD) {
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
		}
		SDL_RenderFillRect( gRenderer, &fillRect );
		
		const int eyeSize = 6;
		SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF ); 
		SDL_Rect rEyeRect;
		SDL_Rect lEyeRect;
		switch (curDir) {
			case EAST:
			rEyeRect = { curX + PLAYER_SIZE - 10, curY + 2 * PLAYER_SIZE / 5 - eyeSize/2, eyeSize, eyeSize};
			lEyeRect = { curX + PLAYER_SIZE - 10, curY + 3 * PLAYER_SIZE / 5 - eyeSize/2, eyeSize, eyeSize};
			break;
			
			case WEST:
			rEyeRect = { curX + 10 - eyeSize, curY + 2 * PLAYER_SIZE / 5 - eyeSize/2, eyeSize, eyeSize};
			lEyeRect = { curX + 10 - eyeSize, curY + 3 * PLAYER_SIZE / 5 - eyeSize/2, eyeSize, eyeSize};
			break;
			
			case NORTH:
			rEyeRect = {curX + 2 * PLAYER_SIZE / 5 - eyeSize/2, curY + 10 - eyeSize, eyeSize, eyeSize};
			lEyeRect = {curX + 3 * PLAYER_SIZE / 5 - eyeSize/2, curY + 10 - eyeSize, eyeSize, eyeSize};
			break;
			
			case SOUTH:
			rEyeRect = {curX + 2 * PLAYER_SIZE / 5 - eyeSize/2, curY + PLAYER_SIZE - 10, eyeSize, eyeSize};
			lEyeRect = {curX + 3 * PLAYER_SIZE / 5 - eyeSize/2, curY + PLAYER_SIZE - 10, eyeSize, eyeSize};
			break;
		}
		SDL_RenderFillRect( gRenderer, &rEyeRect );
		SDL_RenderFillRect( gRenderer, &lEyeRect );
		
		// draw game grid
		// SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		// drawGrid(PLAYER_SIZE);
		
		// render text
		SDL_Rect textQuad = { 10,  5, textWidth, textHeight};
		SDL_RenderCopy( gRenderer, gTextTexture, NULL, &textQuad );
	}
	
	// Update the renderer
	SDL_RenderPresent( gRenderer );
}

int wrap(int val, int mod) {
	int newVal = val;
	if (mod <= 0) {
		newVal  = 0;
	} else {
		newVal = val % mod;
		while (newVal < 0) newVal += mod;
	}
	return newVal;
}

// add max tries
// or manually check number of free squares
bool spawnFood() {
	bool success = false;
	
	int foodX = rand() % GRID_WIDTH;
	int foodY = rand() % GRID_HEIGHT;
	
	if (grid[foodX][foodY] == EMPTY) {
		grid[foodX][foodY] = FOOD;
		success = true;
	}
	
	return success;
}

void updateScore(int newScore) {
	score = newScore;
	if (score > highscore ) {
		highscore = score;
	}
	updateText("Score: " + std::to_string(score) + "\nHighscore: " + std::to_string(highscore));
}

void eat() {
	updateScore(score + 1);					
	if (DELAY > 100) {
		DELAY--;
	}
	
	if (gSoundEnabled) Mix_PlayChannel(-1, gCollect, 0);
	
	while ( !spawnFood() );
}

void die() {
	gameState = DEAD;
	render();
	
	if (gSoundEnabled) Mix_PlayChannel(-1, gDeath, 0);
	
	SDL_Delay(2000);
	gameState = MENU;
	updateText("SPACE to start");
}

void start() {
	curX = SCREEN_WIDTH / 2;
	curY = SCREEN_HEIGHT / 2;
	
	// snap to grid
	curX -= curX % PLAYER_SIZE;
	curY -= curY % PLAYER_SIZE;
	
	curDir = EAST;
	
	for(int i =0; i < GRID_WIDTH; i++) {
		for(int j = 0; j < GRID_HEIGHT; j++) {
			grid[i][j] = EMPTY;
		}
	}
	
	// initial food
	grid[GRID_HEIGHT / 2][2 * GRID_WIDTH/3] = FOOD;
	
	while (!bodyX.empty()){
		bodyX.pop();
	}
	
	while (!bodyY.empty()){
		bodyY.pop();
	}
	
	while (!moves.empty()){
		moves.pop();
	}
	updateScore(0);
	
	gameState = PLAY;
}

void update() {	
	
	if (gameState == PLAY) {	
		int prevHeadX = curX;
		int prevHeadY = curY;
		
		// handle turn
		turn();
		
		// handle move
		switch (curDir) {
			case EAST:
			curX += PLAYER_SPEED;
			break;
			
			case WEST:
			curX -= PLAYER_SPEED;
			break;
			
			case SOUTH:
			curY += PLAYER_SPEED;
			break;
			
			case NORTH:
			curY -= PLAYER_SPEED;
			break;
		}
		
		curX = wrap(curX, SCREEN_WIDTH);
		curY = wrap(curY, SCREEN_HEIGHT);
		
		bool growing = false;
		if (grid[curX / GRID_SIZE][curY / GRID_SIZE] == FOOD) {
			// grow!
			growing = true;
			eat();
		}
		
		if(!growing && !bodyX.empty() && !bodyY.empty()) {			
			// move back of body
			int lastX = bodyX.front();
			bodyX.pop();
			int lastY = bodyY.front();
			bodyY.pop();
			
			grid[lastX / GRID_SIZE][lastY / GRID_SIZE] = EMPTY;
		}
		
		
		bool lost = false;
		// check collisions
		if (grid[curX / GRID_SIZE][curY / GRID_SIZE] == BODY) {
			// lose!
			lost = true;
		}
			
		// add to front of body
		grid[prevHeadX / GRID_SIZE][prevHeadY / GRID_SIZE] = BODY;
		bodyX.push(prevHeadX);
		bodyY.push(prevHeadY);
		
		grid[curX / GRID_SIZE][curY / GRID_SIZE] = HEAD;
		
		if (lost) {
			die();
		}
	}
}

bool handleInput(SDL_Event e) {
	bool quit = false;
	
	// Handle events on queue
	while( SDL_PollEvent( &e ) != 0 )
	{
		// user requests quit
		if( e.type == SDL_QUIT )
		{
			quit = true;
		} else if ( e.type == SDL_KEYUP) {
			printf("Pressed key: %d\n", e.key.keysym.sym);
			
			switch (e.key.keysym.sym) {
				case SDLK_RIGHT:
				if (gameState == PLAY) moves.push(EAST);
				break;
				
				case SDLK_LEFT:
				if (gameState == PLAY) moves.push(WEST);
				break;
				
				case SDLK_UP:
				if (gameState == PLAY) moves.push(NORTH);
				break;
				
				case SDLK_DOWN:
				if (gameState == PLAY) moves.push(SOUTH);
				break;
				
				case SDLK_SPACE:
				if (gameState == MENU) start();
				break;
				
				case SDLK_m:
				gSoundEnabled = !gSoundEnabled;
				break;
				
				case SDLK_ESCAPE:
				quit = true;
				break;
			}
		}
	}
	
	return quit;
}

int main( int argc, char* args[] ) {
	
	// Testing
	// Screen screen(1280, 720);
	// screen.init();
	
	
	// Strat up SDL and create window
	if (!init()) {
		printf("Failed to initialize\n");
	} else {
		// Load media
		if (!loadMedia()) {
			printf("Failed to load media\n");
		} else {
			// Main loop flag
			bool quit = false;
			
			Uint32 prevUpdate = SDL_GetTicks();
			
			// Event handler
			SDL_Event e;
			
			gameState = MENU;
			updateText("SPACE to start");
			
			// Until user quits
			while (!quit) {
				quit = handleInput(e);
				
				Uint32 now = SDL_GetTicks();
				if (now - prevUpdate >= DELAY) {
					
					// Update
					update();
					// Render to screen
					render();
					
					prevUpdate = now;
				}
				
			}
		}
	}
	
	fflush(stdout);

	close();

	return 0;
}