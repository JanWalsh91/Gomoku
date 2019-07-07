#include <Python.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "Minmax.hpp"

Minmax::Minmax(Gomoku& gomoku, int maxDepth): maxDepth(maxDepth), gomoku(gomoku) {
	this->TT = std::map<std::string, int>();
	this->bestMove = std::make_pair(-1, -1);
	this->bestValue = std::numeric_limits<int>::min();

	this->searchDepth = 1;
}


std::vector<std::vector<int>> heuristicValues;

std::pair<int, int> Minmax::run() {
	std::cout << "Minmax run" << std::endl;
	this->gomoku.heuristicPlayer = this->gomoku.currentPlayer;
	this->bestMove = std::make_pair(-1, -1);
	this->bestValue = std::numeric_limits<int>::min() + 1;
	this->TT = std::map<std::string, int>(); // TODO: useless?
	heuristicValues = std::vector<std::vector<int>>(this->gomoku.size, std::vector<int>(this->gomoku.size, 0));
	
	// for (this->searchDepth = 1; this->searchDepth <= this->maxDepth; this->searchDepth++) {
		this->searchDepth = this->maxDepth;
		// TODO: out of time
		std::cout << "search with depth " << this->searchDepth << std::endl;
		this->minmaxAlphaBeta(this->searchDepth, std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max(), 1);

	// }


	this->gomoku.heuristicPlayer = nullptr;

	// std::cout << std::endl << "BOARD" << std::endl;
	// this->gomoku.printBoard();

	std::cout << "heuristicValues for player " << this->gomoku.currentPlayer->index << std::endl;
	this->gomoku.printBoard(heuristicValues);
	std::cout << std::endl;;

	return this->bestMove;
}

bool compareNodes(std::pair<std::pair<int, int>, int> i1, std::pair<std::pair<int, int>, int> i2) { 
    return (i1.second > i2.second);
}

int Minmax::minmaxAlphaBeta(int depth, int alpha, int beta, int player) {
	// std::cout << "minmaxAlphaBeta depth " << depth << std::endl;

	// Get hashKey of parent
	auto key = this->gomoku.hashState();

	// Return early if reached max depth
	if (depth == 0 || this->gomoku.endState != Gomoku::PLAYING) {
		auto keyVal = this->TT.find(key);
		int val;
		if (keyVal == this->TT.end()) {
			// if (this->gomoku.endState != Gomoku::PLAYING) {
			// 	val = 1'000'000;
			// } else {
				val = this->gomoku.heuristic(depth);

			// }

			// val = this->gomoku.endState >= 0 ? val * 100 : val;
			this->TT[key] = val;
		} else {
			val = keyVal->second;
		}
		return player * val;
	}
	
	// Get sorted move from IDTable or generate new ones
	std::vector<std::pair<int, int>> moves;
	std::vector<std::pair<std::pair<int, int>, int>> sortedMoves;
	moves = this->gomoku.getMoves();
	for (auto it = moves.begin(); it != moves.end(); it++) {
		std::vector<AAction*> actions = this->gomoku.doMove(*it);

		// hash and check if in TT
		std::string childKey = this->gomoku.hashState();
		auto keyVal = this->TT.find(key);
		// if (keyVal != this->TT.end()) {
		// 	sortedMoves.push_back(std::make_pair(*it, keyVal->second));
		// } else {
			sortedMoves.push_back(std::make_pair(*it, std::numeric_limits<int>::min()));
		// }
		this->gomoku.undoMove(actions);
	}
	// std::cout << "I AM SEXUALL DISEASILY TRANSMITIDELY C OUTING" <<  std::endl; 
	// for (auto it = sortedMoves.begin(); it != sortedMoves.end(); it++) {
	// 	std::cout << "NOde: [" << std::setw(2) << it->first.first << ", " << std::setw(2) << it->first.second << "] =? " << it->second << std::endl;
	// }
	std::sort(sortedMoves.begin(), sortedMoves.end(), compareNodes);
	// std::cout << "POST SRT SOPONN" << std::endl;
	// for (auto it = sortedMoves.begin(); it != sortedMoves.end(); it++) {
	// 	std::cout << "NOde: [" << std::setw(2) << it->first.first << ", " << std::setw(2) << it->first.second << "] =? " << it->second << std::endl;
	// }

	// if (depth == this->maxDepth) {
	// 	std::cout << "===========" << std::endl;
	// 	for (auto& move: moves) {
	// 		std::cout << "[" << move.first << ", " << move.second << "]" << std::endl;
	// 	}
	// 	std::cout << "===========" << std::endl;
	// }
	int value = std::numeric_limits<int>::min() + 1;

	for (auto it = sortedMoves.begin(); it != sortedMoves.end(); it++) {
		// std::cout << "Eval node: [" << std::setw(2) << it->first.first << ", " << std::setw(2) << it->first.second << "] =? " << it->second << std::endl;
		std::vector<AAction*> actions = this->gomoku.doMove(it->first);
		int ret = -this->minmaxAlphaBeta(depth - 1, -beta, -alpha, -player);

		// std::cout << "after calling minmax" << std::endl;
		if (depth == this->searchDepth) {
			if (depth == this->searchDepth) {
				heuristicValues[it->first.first][it->first.second] = player * ret;
			}
			if (player * ret > this->bestValue) {
				this->bestMove = it->first;
				this->bestValue = player * ret;
				// std::cout << "SET BEST VALUE " <<  "[" << std::setw(2) << this->bestMove.first << ", " << std::setw(2) << this->bestMove.second << "] => " << player * ret << std::endl;
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