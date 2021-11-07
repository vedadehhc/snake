#ifndef SNAKE_H
#define SNAKE_H

#include <utility>
#include <iostream>
#include <queue>
#include <deque>
#include <algorithm>

class Snake {
	
private:
	int id;
	int curDir;
	int score;
	
	// these use grid coords
	std::pair<int, int> head;
	std::deque<std::pair<int, int>> body;
	std::queue<int> turns;
	static std::vector<Snake*> snakes;
	static std::vector<Snake*> toDie;
	
	// toString
	// https://stackoverflow.com/questions/1549930/c-equivalent-of-javas-tostring
	// friend std::ostream& operator<<(std::ostream&, const Snake&);

	
public:
	const static int PLAYER_SIZE = 40;
	const static int PLAYER_SPEED = PLAYER_SIZE;
	Snake(int x, int y);
	
	void addTurn(int turn);
	int turn();
	
	void move();
	void render();
	void eat();
	void die();
	bool isCollided();
	
	static void checkCollisions() {
		toDie.clear();
		
		for (int i = 0; i < snakes.size(); i++) {
			if(snakes[i]->isCollided()) {
				toDie.push_back(snakes[i]);
			}
		}
		
		for (int i = 0; i < toDie.size(); i++) {
			toDie[i]->die();
		}
	}
	
	static void addSnake(Snake* s) {
		snakes.push_back(s);
	}
	
	int getId() { return id; }
};

enum Direction {
	NONE,
	EAST,
	SOUTH,
	NORTH,
	WEST,
	TOTAL
};

int opposite(Direction dir) {
	return Direction::TOTAL - dir;
}

bool isOpposite(Direction a, Direction b) {
	return a != Direction::NONE && b != Direction::NONE && a + b == Direction::TOTAL;
}

#endif