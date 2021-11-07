#ifndef SNAKE_H
#define SNAKE_H

#include <utility>
#include <iostream>
#include <queue>
#include <deque>
#include <algorithm>

#include "direction.h"

class Snake {
	
private:
	int id;
	Direction::Direction curDir;
	int score;
	
	// these use grid coords
	std::pair<int, int> head;
	std::deque<std::pair<int, int>> body;
	std::queue<Direction::Direction> turns;
	static std::vector<Snake*> snakes;
	static std::vector<Snake*> toDie;
	
	// toString
	// https://stackoverflow.com/questions/1549930/c-equivalent-of-javas-tostring
	// friend std::ostream& operator<<(std::ostream&, const Snake&);

	
public:
	const static int PLAYER_SIZE = 40;
	const static int PLAYER_SPEED = PLAYER_SIZE;
	Snake(int x, int y);
	
	/**
	adds a turn to this snake's turn queue
	*/
	void addTurn(Direction::Direction turn);
	
	/**
	attempts to turn using the next turn in the turn queue.
	if a turn is executed, return the new direction.
	otherwise, return direction None
	*/
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
#endif