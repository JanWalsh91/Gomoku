#include "Captures.hpp"

#include <iostream>
Captures::Captures(): ARule("CAPTURES") {}

std::vector<AAction*> Captures::triggerEffects(const Gomoku& gomoku, std::pair<int, int>& pos) {
	std::vector<AAction*> actions;

	return actions;
}

int Captures::checkEndGame(const Gomoku& gomoku) {
	for (auto player: gomoku.players) {
		if (player->getCaptures() >= 5) {
			return player->getIndex();
		}
	}
	return -1;
}

