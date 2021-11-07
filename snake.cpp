#include "snake.h"
#include "screen.h"
#include "grid.h"
#include <iostream>
#include <utility>
#include <algorithm>

std::vector<Snake*> Snake::snakes;
std::vector<Snake*> Snake::toDie;

Snake::Snake(int x, int y) {
	head = std::make_pair(x, y);
	addSnake(this);
}

void Snake::addTurn(int turn) {
	// TODO: implement
}

int Snake::turn() {
	// TODO: implement
	bool moved = false;
	if(!moves.empty()) {
		int move = moves.front();
		moves.pop();
		
		if (move + curDir != TOTAL - 1) {
			curDir = move;
		}
		
		moved = true;
	}
	return moved;
	std::cout << "turning!" << std::endl;
	return 0;
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
		case EAST:
		head.first += PLAYER_SPEED;
		break;
		
		case WEST:
		head.first -= PLAYER_SPEED;
		break;
		
		case SOUTH:
		head.second += PLAYER_SPEED;
		break;
		
		case NORTH:
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