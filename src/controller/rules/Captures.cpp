#include "Captures.hpp"

Captures::Captures(): ARule("CAPTURES") {}

std::vector<AAction*> Captures::triggerEffects(Gomoku& gomoku, std::pair<int, int>& move) {

	std::vector<AAction*> actions;

	int otherPlayer = gomoku.currentPlayer->getIndex() == 0 ? 1 : 0;
	std::vector<std::pair<int, int>> directions = {
		std::make_pair( 0,  1),
		std::make_pair( 0, -1),
		std::make_pair( 1,  0),
		std::make_pair(-1,  0),
		std::make_pair( 1,  1),
		std::make_pair(-1, -1),
		std::make_pair(-1,  1),
		std::make_pair( 1, -1),
	};

	for (auto& direction : directions) {
		std::pair<int, int> pos1 = move + direction;
		if (gomoku.notOnBoard(pos1) || gomoku.getValueOnBoard(pos1) != otherPlayer) {
			continue ;
		}
		std::pair<int, int> pos2 = pos1 + direction;
		if (gomoku.notOnBoard(pos2) || gomoku.getValueOnBoard(pos2) != otherPlayer) {
			continue ;
		}
		std::pair<int, int> pos3 = pos2 + direction;
		if (gomoku.notOnBoard(pos3) || gomoku.getValueOnBoard(pos3) != gomoku.currentPlayer->getIndex()) {
			continue ;
		}

		// udpate board
		gomoku.setValueOnBoard(pos1, -1);
		gomoku.setValueOnBoard(pos2, -1);
		gomoku.updateBoardCallbacks(pos1, -1);
		gomoku.updateBoardCallbacks(pos2, -1);
		gomoku.setRemainingStones(gomoku.getRemainingStones() + 2);

		// give points
		gomoku.currentPlayer->incrementCaptures();
		gomoku.captureCallbacks(gomoku.currentPlayer->getIndex());
		
		// prepare undo actions
		actions.push_back(new ActionDecrementCapture(gomoku.currentPlayer->getIndex()));
		actions.push_back(new ActionUpdateBoard(pos1, otherPlayer));
		actions.push_back(new ActionUpdateBoard(pos2, otherPlayer));
	}
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

