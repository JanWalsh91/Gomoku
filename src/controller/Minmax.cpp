#include <Python.h>
#include <iostream>
#include <sstream>

#include "Minmax.hpp"

Minmax::Minmax(Gomoku& gomoku, int maxDepth): maxDepth(maxDepth), gomoku(gomoku) {
	this->TT = std::map<std::string, int>();
	this->bestMove = std::make_pair(-1, -1);
	this->bestValue = std::numeric_limits<int>::min();
}


std::vector<std::vector<int>> heuristicValues;

std::pair<int, int> Minmax::run() {
	// std::cout << "Minmax run" << std::endl;
	this->gomoku.heuristicPlayer = this->gomoku.currentPlayer;
	this->bestMove = std::make_pair(-1, -1);
	this->bestValue = std::numeric_limits<int>::min() + 1;
	this->TT = std::map<std::string, int>();
	
	heuristicValues = std::vector<std::vector<int>>(this->gomoku.size, std::vector<int>(this->gomoku.size, 0));
	// std::cout << "==================================================" << std::endl;
	// std::cout << "==================================================" << std::endl;
	this->minmaxAlphaBeta(this->maxDepth, std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max(), 1);
	this->gomoku.heuristicPlayer = nullptr;

	// std::cout << std::endl << "BOARD" << std::endl;
	// this->gomoku.printBoard();
	// std::cout << "heuristicValues" << std::endl;;
	// this->gomoku.printBoard(heuristicValues);
	// std::cout << std::endl;;

	return this->bestMove;
}

int Minmax::minmaxAlphaBeta(int depth, int alpha, int beta, int player) {
	// std::cout << "minmaxAlphaBeta " << alpha << "    " << beta << std::endl;

	if (depth == this->maxDepth) {
		std::cout << "End State: " << this->gomoku.endState << std::endl;
	}

	if (depth == 0 || this->gomoku.endState >= 0) {
		auto key = this->gomoku.hashState();
		auto keyVal = this->TT.find(key);
		int val;
		if (keyVal == this->TT.end()) {
			val = this->gomoku.heuristic();
			// val = this->gomoku.endState >= 0 ? 1000000 : this->gomoku.heuristic();
			this->TT[key] = val;
		} else {
			val = keyVal->second;
		}
		return player * val;
	} 

	auto moves = this->gomoku.getMoves();
	int value = std::numeric_limits<int>::min() + 1;

	for (auto it = moves.begin(); it != moves.end(); it++) {
		// TODO: out of time
		std::vector<AAction*> actions = this->gomoku.doMove(*it);
		// std::cout << "Before calling minmax" << std::endl;
		int ret = -this->minmaxAlphaBeta(depth - 1, -beta, -alpha, -player);
		// std::cout << "after calling minmax" << std::endl;
		if (depth == this->maxDepth || this->gomoku.endState >= 0) {
			if (this->gomoku.endState >= 0) {
				// std::cout << "RET: " << ret << std::endl;
			}
			heuristicValues[it->first][it->second] = player * ret;
			if (player * ret > this->bestValue || this->gomoku.endState >= 0) {
				// std::cout << "SET BEST VALUE " << std::endl;
				this->bestMove = *it;
				this->bestValue = player * ret;
			}
		}

		value = std::max(value, ret);
		this->gomoku.undoMove(actions);
		alpha = std::max(alpha, value);
		// std::cout << "value: " << value << std::endl;
		// std::cout << "alpha: " << alpha << std::endl;
		// std::cout << "beta: " << beta << std::endl;
		if (alpha >= beta) {
			// std::cout << "BREAKING NEWS" << std::endl;
			break;
		}
	}

	return value;
}