#ifndef GRID_H
#define GRID_H

namespace grid {		
	#include "snake.h"
	#include "screen.h"

	// perhaps do something like:
	// empty = 0
	// each body/head = +1


	const int GRID_WIDTH = 32;
	const int GRID_HEIGHT = 18;
	// computed using grid width/height and screen sizefor compatability
	const int GRID_SIZE = std::min(screen::SCREEN_WIDTH / GRID_WIDTH, screen::SCREEN_HEIGHT / GRID_HEIGHT); 
	
	
	// for now, everything is in the top left, but these should be updated for centering later
	int screenToGridX(int x) {
		return x / GRID_WIDTH;
	}

	int screenToGridY(int y) {
		return y / GRID_HEIGHT;
	}

	int gridToScreenX(int x) {
		return x * GRID_WIDTH;
	}

	int gridToScreenY(int y) {
		return y * GRID_HEIGHT;
	}
	
	int wrap(int n, int mod) {
		int y = n % mod;
		while (y < 0) y += mod;
		return y;
	}

	int wrapX(int x) {
		return wrap(x, GRID_WIDTH);
	}
	
	int wrapY(int y) {
		return wrap(y, GRID_HEIGHT);
	}
	
	// counts number of snake heads/bodies in this cell
	int snakeGrid[GRID_WIDTH][GRID_HEIGHT] = {};
	// whether there is food in this cell
	bool foodGrid[GRID_WIDTH][GRID_HEIGHT] = {};
};

#endif