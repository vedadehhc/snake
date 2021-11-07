#ifndef GAME_H
#define GAME_H

#include <iostream>

#include "Screen.h"
#include "Grid.h"

class Game {
private:
	// how big each cell is, in screen units
	int cellSize;
	
	
public:
	Screen screen;
	Grid grid;
	Game() : screen(1280, 720), grid(32, 18) {
		
		// computed using grid width/height and screen size for compatability
		cellSize = std::min(screen.getWidth() / grid.getWidth(), screen.getHeight() / grid.getHeight()); 
	}

	// for now, everything is in the top left, but these should be updated for centering later
	int screenToGridX(int x) {
		return x / cellSize;
	}

	int screenToGridY(int y) {
		return y / cellSize;
	}

	int gridToScreenX(int x) {
		return x * cellSize;
	}

	int gridToScreenY(int y) {
		return y * cellSize;
	}
};

#endif