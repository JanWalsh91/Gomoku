#pragma once

#include "Common.hpp"

class Gomoku;

class ActionIncrementCapture: public AAction {

public:

	ActionIncrementCapture(int playerIndex);

	~ActionIncrementCapture();
	void execute(Gomoku& gomoku);


private:
	int _playerIndex;
};