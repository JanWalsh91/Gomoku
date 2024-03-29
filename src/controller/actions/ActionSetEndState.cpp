#include "ActionSetEndState.hpp"

ActionSetEndState::ActionSetEndState(int state): _state(state) {}
ActionSetEndState::~ActionSetEndState() {}

void ActionSetEndState::execute(Gomoku& gomoku) {
	gomoku.setEndState(_state);
}