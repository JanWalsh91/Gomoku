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
			pos = std::make_pair(move.first + dir.first * i, move.second + dir.second * i);
			if (pos.first > gomoku.size - 1 || pos.second > gomoku.size - 1 || pos.first < 0 || pos.second < 0) {
				break;
			}
			line[lineIndex + i] = gomoku.board[pos.first][pos.second];
		}
		
		for (int i = -1; i > -5; i--) {
			pos = std::make_pair(move.first + dir.first * i, move.second + dir.second * i);
			if (pos.first > gomoku.size - 1 || pos.second > gomoku.size - 1 || pos.first < 0 || pos.second < 0) {
				break;
			}
			line[lineIndex + i] = gomoku.board[pos.first][pos.second];
		}

		// std::cout << "Line: ";
		// for (int p = 0; p < line.size(); p++) {
		// 	std::cout << line[p] << " ";
		// }
		// std::cout << std::endl;

		for (int i = 1; i < 5; i++) {
			subLine = std::vector<int>(line.begin() + i, line.begin() + i + 4);
			int numCurrentPlayer = std::count_if(subLine.begin(), subLine.end(), [currentPlayer](int v){ return v == currentPlayer; });
			int numEmpty = std::count_if(subLine.begin(), subLine.end(), [](int v){ return v == -1; });
	
			// std::cout << "Subline: ";
			// for (int p = 0; p < subLine.size(); p++) {
			// 	std::cout << subLine[p] << " ";
			// }
			// std::cout << std::endl;
			// std::cout << "numCurrentPlayer: " << numCurrentPlayer << std::endl;
			// std::cout << "numEmpty: " << numEmpty << std::endl;

			if (numCurrentPlayer == 3 && numEmpty == 1) {
				
				auto first = std::find(subLine.begin(), subLine.end(), currentPlayer);
				auto last = std::find(subLine.rbegin(), subLine.rend(), currentPlayer);

				// std::cout << "\tFirst: " << *first << std::endl;
				// std::cout << "\tLast: " << *last << std::endl;


				int indexOfFirst = first - subLine.begin();
				int indexOfLast =  subLine.rend() - last - 1;

				// std::cout << "\tFirst index: " << indexOfFirst << std::endl;
				// std::cout << "\tLast index: " << indexOfLast << std::endl;

				// std::cout << "\tBefore first: " << line[indexOfFirst + i - 1] << std::endl;
				// std::cout << "\tAfter last: " << line[indexOfLast + i + 1] << std::endl;

				if (line[indexOfFirst + i - 1] == -1 && line[indexOfLast + i + 1] == -1) {
					numFreeThrees++;
					break;
				}
			}
		}
	}
	// std::cout << name << ": " << move << " canPlace? numFreeThrees: " << numFreeThrees << std::endl;
	return numFreeThrees < 2;
}
