#pragma once

#include "Common.hpp"

class AAction;

class ActionSetEndState: public AAction {

public:

	ActionSetEndState(int state);
	
	~ActionSetEndState();
	void execute(Gomoku& gomoku);

private:
	int _state;
};