#include "NoDoubleFreeThree.hpp"

#include <iostream>
NoDoubleFreeThree::NoDoubleFreeThree(): ARule("NO DOUBLE FREE THREE") {}

bool NoDoubleFreeThree::canPlace(const Gomoku& gomoku, std::pair<int, int>& move) {
	return this->canPlace(gomoku, move, gomoku.currentPlayer->getIndex());
}

bool NoDoubleFreeThree::canPlace(const Gomoku& gomoku, std::pair<int, int>& move, int currentPlayer) {
	std::vector<std::pair<int, int>> directions = {
		std::make_pair(0,  1),
		std::make_pair(1,  0),
		std::make_pair(1,  1),
		std::make_pair(-1, 1),
	};
	int numFreeThrees = 0;
	std::vector<int> line(9);
	std::vector<int> subLine(4);
	int otherPlayer = currentPlayer == 0 ? 1 : 0;
	int lineIndex;
	std::pair<int, int> pos;

	for (std::pair<int, int>& dir: directions) {
		if (numFreeThrees >= 2) {
			break;
		}
		std::fill(line.begin(), line.end(), otherPlayer);
		lineIndex = 4;
		line[lineIndex] = currentPlayer;

		for (int i = 1; i < 5; i++) {
			pos = move + dir * i;
			if (gomoku.notOnBoard(pos)) {
				break;
			}
			line[lineIndex + i] = gomoku.getValueOnBoard(pos);
		}
		
		for (int i = -1; i > -5; i--) {
			pos = move + dir * i;
			if (gomoku.notOnBoard(pos)) {
				break;
			}
			line[lineIndex + i] = gomoku.getValueOnBoard(pos);
		}

		for (int i = 1; i < 5; i++) {
			subLine = std::vector<int>(line.begin() + i, line.begin() + i + 4);
			int numCurrentPlayer = std::count_if(subLine.begin(), subLine.end(), [currentPlayer](int v){ return v == currentPlayer; });
			int numEmpty = std::count_if(subLine.begin(), subLine.end(), [](int v){ return v == -1; });
	
			if (numCurrentPlayer == 3 && numEmpty == 1) {
				
				auto first = std::find(subLine.begin(), subLine.end(), currentPlayer);
				auto last = std::find(subLine.rbegin(), subLine.rend(), currentPlayer);

				int indexOfFirst = first - subLine.begin();
				int indexOfLast =  subLine.rend() - last - 1;

				if (line[indexOfFirst + i - 1] == -1 && line[indexOfLast + i + 1] == -1) {
					numFreeThrees++;
					break;
				}
			}
		}
	}
	return numFreeThrees < 2;
}
