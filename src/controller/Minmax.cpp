#include <Python.h>
#include <iostream>
#include <sstream>

#include "Minmax.hpp"

#define DEPTH 1

Minmax::Minmax(Gomoku& gomoku): gomoku(gomoku) {
	this->TT = std::map<std::string, int>();
	this->bestMove = std::make_pair(-1, -1);
	this->bestValue = std::numeric_limits<int>::min();
}

std::pair<int, int> Minmax::run() {
	std::cout << "Minmax run" << std::endl;
	this->gomoku.heuristicPlayer = this->gomoku.currentPlayer;
	this->TT = std::map<std::string, int>();
	
	this->minmaxAlphaBeta(DEPTH, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 1);
	this->gomoku.heuristicPlayer = nullptr;

	return this->bestMove;
}

int Minmax::minmaxAlphaBeta(int depth, int alpha, int beta, int player) {
	std::cout << "minmaxAlphaBeta" << std::endl;

	if (depth == 0) {
		auto key = gomoku.hashState();
		auto keyVal = this->TT.find(key);
		int val;
		if (keyVal == this->TT.end()) {
			val = this->gomoku.heuristic();
			this->TT[key] = val;
		} else {
			val = keyVal->second;
		}
		return player * val;
	} 

	auto moves = this->gomoku.getMoves();
	int value = std::numeric_limits<int>::max();

	for (auto it = moves.begin(); it != moves.end(); it++) {
		// TODO: out of time
		this->gomoku.doMove(*it);
		int ret = -this->minmaxAlphaBeta(depth - 1, -beta, -alpha, -player);
		if (depth == DEPTH) { // TODO:!!!!!!!!!!!!!!!!! max depth
			if (player * ret > this->bestValue) {
				this->bestValue = player * ret;
				this->bestMove = *it;
			}
		}

		value = std::max(value, ret);
		gomoku.undoMove(*it);
		alpha = std::max(alpha, value);
		if (alpha >= beta) {
			break;
		}
	}

	return value;
}