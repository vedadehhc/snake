#ifndef DIRECTION_H
#define DIRECTION_H

#include <string>

namespace Direction {
	enum Direction {
		NONE = 0,
		EAST = 1,
		SOUTH = 2,
		NORTH = 3,
		WEST = 4,
		TOTAL = 5,
	};
	
	static const std::string DirectionNames[] = {
		"NONE",
		"EAST",
		"SOUTH",
		"NORTH",
		"WEST"
	};
	
	// statically check that the size of ColorNames fits the number of Colors
	static_assert(sizeof(DirectionNames)/sizeof(std::string) == Direction::TOTAL, "sizes dont match");

	Direction oppositeDirection(Direction dir) {
		return static_cast<Direction>(Direction::TOTAL - dir);
	}

	bool isOppositeDirection(Direction a, Direction b) {
		return a != Direction::NONE && b != Direction::NONE && a + b == Direction::TOTAL;
	}
}

#endif