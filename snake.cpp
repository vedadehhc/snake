#include <iostream>
#include <utility>
#include <algorithm>

#include "snake.h"
#include "grid.h"
#include "direction.h"

std::vector<Snake*> Snake::snakes;
std::vector<Snake*> Snake::toDie;

Snake::Snake(Grid* g, int x, int y) {
	grid = g;
	head = std::make_pair(x, y);
	addSnake(this);
	curDir = Direction::EAST;
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

bool Snake::eat() {
	if (grid->eatFood(head.first, head.second)) {
		score++;
		return true;
	}
	return false;
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
	head.first = grid->wrapX(head.first);
	head.second = grid->wrapY(head.second);
	
	// try to eat
	bool growing = eat();
	
	// issue with setting grid directly if one snake enters square and another leaves it,
	// will be marked as empty instead of head
	// fixed by using grid as cound of bodies?
	
	if(!growing && !body.empty()) {			
		// move back of body
		std::pair<int, int> lastPos = body.front();
		body.pop_front();
		
		grid->updateSnakeGrid(lastPos.first, lastPos.second, -1);
	}
	
	if (growing || !body.empty()) {	
		// add to front of body
		// grid::snakeGrid[prevHeadX][prevHeadY] += 1; // not needed since head was there previously
		body.push_back(std::make_pair(prevHeadX, prevHeadY));
	}
	
	grid->updateSnakeGrid(head.first, head.second, 1);
}

void Snake::die() {
	grid->updateSnakeGrid(head.first, head.second, -1);
	while (!body.empty()) {
		std::pair<int, int> cell = body.front();
		body.pop_front();
		
		grid->updateSnakeGrid(cell.first, cell.second, -1);
	}
}

bool Snake::isCollided() {
	return grid->numSnakesAtPos(head.first, head.second) > 1;
}


int main() {
	
	Grid g(4,4);
	
	Snake s (&g, g.getWidth()/2, g.getHeight()/2);
	
	s.render();
	
	std::cout << "moving" << std::endl;
	s.move();
	
	s.render();
	
	s.addTurn(Direction::NORTH);
	
	
	std::cout << "moving" << std::endl;
	
	s.move();
	
	s.render();
	
	
	
	return 0;
}