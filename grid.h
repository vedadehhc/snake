#ifndef GRID_H
#define GRID_H

#include <random>
#include <chrono>

class Grid {
	
private:
	int width;
	int height;
	int size;
	
	// perhaps do something like:
	// empty = 0
	// each body/head = +1
	// counts number of snake heads/bodies in this cell
	std::vector<int> snakeGrid;
	
	// whether there is food in this cell
	std::vector<bool> foodGrid;
	
	// const int GRID_SIZE = std::min(screen::SCREEN_WIDTH / GRID_WIDTH, screen::SCREEN_HEIGHT / GRID_HEIGHT); 
	
	// random generator
	std::default_random_engine generator;
	
public:	
	// const int GRID_WIDTH = 32;
	// const int GRID_HEIGHT = 18;
	
	Grid(int w, int h) {
		width = w;
		height = h;
		size = w*h;
		
		for (int i = 0; i < size; i++) {
			snakeGrid.push_back(0);
			foodGrid.push_back(false);
		}
		
		// assert (snakeGrid.size() == size);
		// assert (foodGrid.size() == size);
		
		generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
		
		// std::cout << "made grid: " << w << ", " << h << std::endl;
	}
	
	// getters
	int getWidth() {
		return width;
	}
	
	int getHeight() {
		return height;
	}
	
	int wrap(int n, int mod) {
		int y = n % mod;
		while (y < 0) y += mod;
		return y;
	}

	int wrapX(int x) {
		return wrap(x, width);
	}
	
	int wrapY(int y) {
		return wrap(y, height);
	}
	
	int flatIndex(int x, int y) {
		return y * width + x;
	}
	
	/**
	updates snakeGrid at (x,y) by amt.
	*/
	void updateSnakeGrid(int x, int y, int amt) {
		int i = flatIndex(x, y);
		if (0 <= i && i < size) {
			snakeGrid[i] += amt;
		}
	}
	
	/**
	returns the value of snakeGrid at (x,y).
	*/
	int numSnakesAtPos(int x, int y) {
		int i = flatIndex(x, y);
		if (0 <= i && i < size) {
			return snakeGrid[i];
		}
		return 0;
	}
	
	/**
	spawn food at a random unoccupied position.
	returns true if spawn was successful.
	*/
	bool spawnFood() {
		// count empty spots
		int numEmpty = 0;
		for (int i = 0; i < size; i++) {
			if (snakeGrid[i] == 0) numEmpty++;
		}
		
		if (numEmpty == 0) return false;
		// generate random food location
		std::uniform_int_distribution<int> distribution(0, numEmpty-1);
		
		int foodLoc = distribution(generator);
		
		for (int i = 0; i < size; i++) {
			if (snakeGrid[i] == 0)  {
				numEmpty--;
				if (numEmpty == foodLoc) {
					foodGrid[i] = true;
					return true;
				}
			}
		}
		return false;
	}
	
	/**
	returns whether there is food at (x,y)
	*/
	bool hasFood(int x, int y) {
		int i = flatIndex(x, y);
		if (0 <= i && i < size) {
			return foodGrid[i];
		}
		return false;
	}
	
	/**
	attempts to eat food at (x,y). if there is food at this pos,
	returns 1. otherwise return 0.
	*/
	int eatFood(int x, int y) {
		int i = flatIndex(x, y);
		if (0 <= i && i < size) {
			if (foodGrid[i]) {
				foodGrid[i] = false;
				return 1;
			}
			return 0;
		}
		return 0;
	}
};

#endif