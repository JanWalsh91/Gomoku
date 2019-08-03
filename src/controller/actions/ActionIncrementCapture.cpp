#include "ActionIncrementCapture.hpp"

ActionIncrementCapture::ActionIncrementCapture(int playerIndex): _playerIndex(playerIndex) {}
ActionIncrementCapture::~ActionIncrementCapture() {}

void ActionIncrementCapture::execute(Gomoku& gomoku) {
	gomoku.players[_playerIndex]->incrementCaptures();
}