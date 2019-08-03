#pragma once

#include "Common.hpp"

class Gomoku;

class ActionDecrementCapture: public AAction {

public:

	ActionDecrementCapture(int playerIndex);

	~ActionDecrementCapture();
	void execute(Gomoku& gomoku);


private:
	int _playerIndex;
};