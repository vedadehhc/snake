#include <iostream>
#include <utility>
#include <algorithm>

#include "snake.h"
#include "screen.h"
#include "grid.h"
#include "direction.h"

std::vector<Snake*> Snake::snakes;
std::vector<Snake*> Snake::toDie;

Snake::Snake(int x, int y) {
	head = std::make_pair(x, y);
	addSnake(this);
}

void Snake::addTurn(Direction::Direction turn) {
	turns.push(turn);
}

int Snake::turn() {
	Direction::Direction newDir = Direction::NONE;
	if(!turns.empty()) {
		Direction::Direction turn = turns.front();
		turns.pop();
		
		if (!isOppositeDirection(turn, curDir) && turn != curDir) {
			curDir = turn;
			newDir = turn;
		}
	}
	return newDir;
}

void Snake::eat() {
	if (grid::foodGrid[head.first][head.second]) {
		score++;
	}
	
	grid::foodGrid[head.first][head.second] = false;
}

void Snake::render() {
	// TODO: implement
	std::cout << head.first << " " << head.second << std::endl;
	std::for_each(
		body.begin(),
		body.end(),
		[](std::pair<int, int> &cell) {
			std::cout << cell.first << " " << cell.second << std::endl;
		}
	);
}

void Snake::move() {
	int prevHeadX = head.first;
	int prevHeadY = head.second;
	
	// handle turn
	turn();
	
	
	// handle move
	switch (curDir) {
		case Direction::EAST:
		head.first += PLAYER_SPEED;
		break;
		
		case Direction::WEST:
		head.first -= PLAYER_SPEED;
		break;
		
		case Direction::SOUTH:
		head.second += PLAYER_SPEED;
		break;
		
		case Direction::NORTH:
		head.second -= PLAYER_SPEED;
		break;
	}
	
	// wrap head to grid
	head.first = grid::wrapX(head.first);
	head.second = grid::wrapY(head.second);
	
	// check eating
	bool growing = false;
	if (grid::foodGrid[head.first][head.second]) {
		// grow!
		growing = true;
		eat();
	}
	
	// issue with setting grid directly if one snake enters square and another leaves it,
	// will be marked as empty instead of head
	// fixed by using grid as cound of bodies?
	
	if(!growing && !body.empty()) {			
		// move back of body
		std::pair<int, int> lastPos = body.front();
		body.pop_front();
		
		grid::snakeGrid[lastPos.first][lastPos.second] -= 1;
	}
	
	// add to front of body
	// grid::snakeGrid[prevHeadX][prevHeadY] += 1; // not needed since head was there previously
	body.push_back(std::make_pair(prevHeadX, prevHeadY));
	
	grid::snakeGrid[head.first][head.second] += 1;
}

void Snake::die() {
	grid::snakeGrid[head.first][head.second] -= 1;
	while (!body.empty()) {
		std::pair<int, int> cell = body.front();
		body.pop_front();
		
		grid::snakeGrid[cell.first][cell.second] -= 1;
	}
}

bool Snake::isCollided() {
	return grid::snakeGrid[head.first][head.second] > 1;
}


int main() {
	
	Snake s (0, 0);
	
	s.render();
	
	return 0;
}