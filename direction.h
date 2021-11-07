#ifndef DIRECTION_H
#define DIRECTION_H

namespace Direction {
	enum Direction {
		NONE = 0,
		EAST = 1,
		SOUTH = 2,
		NORTH = 3,
		WEST = 4,
		TOTAL = 5,
	};

	Direction oppositeDirection(Direction dir) {
		return static_cast<Direction>(Direction::TOTAL - dir);
	}

	bool isOppositeDirection(Direction a, Direction b) {
		return a != Direction::NONE && b != Direction::NONE && a + b == Direction::TOTAL;
	}
}

#endif