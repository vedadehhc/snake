#ifndef SCREEN_H
#define SCREEN_H

class Screen {
private:
	// screen dimensions
	int width, height;
	
public:
	Screen(int screenWidth, int screenHeight) {
		width = screenWidth;
		height = screenHeight;
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