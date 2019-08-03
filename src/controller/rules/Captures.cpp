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
		std::pair<int, int> pos1 = std::make_pair(move.first + direction.first, move.second + direction.second);
		if (pos1.first < 0 || pos1.first >= gomoku.size || pos1.second < 0 || pos1.second >= gomoku.size || gomoku.board[pos1.first][pos1.second] != otherPlayer) {
			continue ;
		}
		std::pair<int, int> pos2 = std::make_pair(pos1.first + direction.first, pos1.second + direction.second);
		if (pos2.first < 0 || pos2.first >= gomoku.size || pos2.second < 0 || pos2.second >= gomoku.size || gomoku.board[pos2.first][pos2.second] != otherPlayer) {
			continue ;
		}
		std::pair<int, int> pos3 = std::make_pair(pos2.first + direction.first, pos2.second + direction.second);
		if (pos3.first < 0 || pos3.first >= gomoku.size || pos3.second < 0 || pos3.second >= gomoku.size || gomoku.board[pos3.first][pos3.second] != gomoku.currentPlayer->getIndex()) {
			continue ;
		}

		// udpate board
		gomoku.board[pos1.first][pos1.second] = -1;
		gomoku.board[pos2.first][pos2.second] = -1;
		gomoku.updateBoardCallbacks(pos1, -1);
		gomoku.updateBoardCallbacks(pos2, -1);
		gomoku.remainingStones += 2;

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

