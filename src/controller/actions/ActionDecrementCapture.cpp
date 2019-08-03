#include "ActionDecrementCapture.hpp"

ActionDecrementCapture::ActionDecrementCapture(int playerIndex): _playerIndex(playerIndex) {}
ActionDecrementCapture::~ActionDecrementCapture() {}

void ActionDecrementCapture::execute(Gomoku& gomoku) {
	gomoku.players[_playerIndex]->decrementCaptures();
}