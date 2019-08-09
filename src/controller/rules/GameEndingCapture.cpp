#include "GameEndingCapture.hpp"

GameEndingCapture::GameEndingCapture(): ARule("GAME ENDING CAPTURE") {}

int GameEndingCapture::checkEndGame(const Gomoku& gomoku) {

	if (gomoku.players[gomoku.currentPlayer->getIndex() == 0 ? 1 : 0]->getCaptures() != 4 ||
		gomoku.players[gomoku.currentPlayer->getIndex() == 0 ? 1 : 0]->getPotentialCaptures() == 0) {

		return gomoku.currentPlayer->getIndex();
	}
	return -1;
}
