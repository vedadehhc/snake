#ifndef SNAKE_H
#define SNAKE_H

#include <utility>
#include <iostream>
#include <queue>
#include <deque>
#include <algorithm>

#include "Direction.h"
#include "Grid.h"
#include "Screen.h"

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
	
	Grid& grid;
	// toString
	// https://stackoverflow.com/questions/1549930/c-equivalent-of-javas-tostring
	// friend std::ostream& operator<<(std::ostream&, const Snake&);

	
public:
	const static int PLAYER_SIZE = 40;
	const static int PLAYER_SPEED = 1;
	Snake(Grid& g, int x, int y);
	
	/**
	renders the snake to the given screen
	*/
	void render(Screen& s);
	
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
	
	/**
	moves the snake in the direction it is currentlty facing.
	also moves the body pieces accordingly. if the snake encounters
	food, it eats the food and grows the body.
	*/
	void move();
	
	/**
	tries to eat at the current head of the snake. if eating is successful 
	(i.e. there was food at the head) return true.
	*/
	bool eat();
	
	/**
	kills this snake.
	*/
	void die();
	
	/**
	checks whether this snake's head has collided with any other snakes or 
	itself.
	*/
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
		s->id = snakes.size();
		snakes.push_back(s);
	}
	
	int getId() { return id; }
};
#endif