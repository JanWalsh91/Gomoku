#pragma once

#include "Common.hpp"

class Gomoku;

class ActionUpdateBoard: public AAction {

public:

	ActionUpdateBoard(std::pair<int, int> pos, int value);

	~ActionUpdateBoard();
	
	void execute(Gomoku& gomoku);
private:

	std::pair<int, int> _pos;
	int _value;

};