#include <Python.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "Minmax.hpp"

#include <chrono>
#include <random>       // std::default_random_engine

Minmax::Minmax(Gomoku& gomoku, int maxDepth): maxDepth(maxDepth), gomoku(gomoku) {
	this->TT = std::map<std::string, Minmax::Entry>();
	this->bestMove = std::make_pair(-1, -1);
	this->bestValue = std::numeric_limits<int>::min();
}


std::vector<std::vector<int>> heuristicValues;

std::pair<int, int> Minmax::run() {
	std::cout << "Minmax run " << this->gomoku.currentPlayer->index << std::endl;
	this->gomoku.heuristicPlayer = this->gomoku.currentPlayer;
	
	this->bestMove = std::make_pair(-1, -1);
	this->bestValue = Minmax::INF_MIN;
	
	heuristicValues = std::vector<std::vector<int>>(this->gomoku.size, std::vector<int>(this->gomoku.size, 0));
	
	auto moves = this->gomoku.getMoves();
	
	for (auto &move: moves) {
		auto undoMoves = this->gomoku.doMove(move);

		int ret = this->minmaxAlphaBeta(this->maxDepth - 1, Minmax::INF_MIN, Minmax::INF_MAX, true);
		if (ret > this->bestValue) {
			this->bestMove = move;
			this->bestValue = ret;
		}
		heuristicValues[move.first][move.second] = ret;

		this->gomoku.undoMove(undoMoves);
	}


	std::cout << "heuristicValues for player " << this->gomoku.currentPlayer->index << std::endl;
	this->gomoku.printBoard(heuristicValues);
	std::cout << std::endl;

	return this->bestMove;
}

int Minmax::minmaxAlphaBeta(int depth, int alpha, int beta, bool maximizing) {

	if (this->gomoku.endState != Gomoku::PLAYING) {
		// Player 0 or 1 :)
		if (this->gomoku.endState >= 0) {
			if (maximizing) {
				return Minmax::INF_MAX - (this->maxDepth - depth);
			} else {
				return Minmax::INF_MIN + (this->maxDepth - depth);
			}
		} else {
			// Draw
			return 0;
		}
	}

	if (depth == 0) {
		return this->gomoku.heuristic();
	}

	auto moves = this->gomoku.getMoves();
	moves = this->getSortedMoves(moves, maximizing, depth);

	if (maximizing) {

		int m = Minmax::INF_MAX;

		for (auto& move: moves) {
			auto undoMoves = this->gomoku.doMove(move);
			int ret = this->minmaxAlphaBeta(depth - 1, alpha, beta, false);
			this->gomoku.undoMove(undoMoves);		

			if (m > ret) {
				m = ret;
			}
			if (beta > m) {
				beta = m;
			}
			if (alpha >= beta) {
				// break;
			}
		}
		return m;
	} else {

		int M = Minmax::INF_MIN;

		for (auto& move: moves) {
			auto undoMoves = this->gomoku.doMove(move);
			int ret = this->minmaxAlphaBeta(depth - 1, alpha, beta, true);
			this->gomoku.undoMove(undoMoves);		

			if (M < ret) {
				M = ret;
			}
			if (alpha < M) {
				alpha = M;
			}
			if (alpha >= beta) {
				// break;
			}
		}
		return M;
	}

}

int		Minmax::scaleByDepth(int depth, int value) {
	// return value * (depth + 1);
	return value;
}

std::vector<std::pair<int, int>> Minmax::getSortedMoves(std::vector<std::pair<int, int>>& moves, bool maximizing, int depth) const {
	auto sortedMoves = std::vector<std::pair<int, int>>();
	
	std::vector<int> movesHeuristicValues;
	for (auto& move: moves) {
		auto undoMoves = this->gomoku.doMove(move);

		if (this->gomoku.endState >= 0) {
			if (maximizing) {
				movesHeuristicValues.push_back(Minmax::INF_MAX - (this->maxDepth - depth));
			} else {
				movesHeuristicValues.push_back(Minmax::INF_MIN + (this->maxDepth - depth));
			}
		} else if (this->gomoku.endState == Gomoku::State::DRAW) {
			movesHeuristicValues.push_back(0);
		} else {
			movesHeuristicValues.push_back(this->gomoku.heuristic());
		}
		
		this->gomoku.undoMove(undoMoves);
	}
	while (movesHeuristicValues.size()) {
		int maxValue = std::numeric_limits<int>::min();
		int maxValueIndex = -1;
		for (unsigned i = 0; i < moves.size(); i++) {
			if (movesHeuristicValues[i] > maxValue) {
				maxValueIndex = i;
				maxValue = movesHeuristicValues[i];
			}
		}
		sortedMoves.push_back(moves[maxValueIndex]);
		movesHeuristicValues.erase(movesHeuristicValues.begin() + maxValueIndex);
		moves.erase(moves.begin() + maxValueIndex);
	}
	return sortedMoves;
}
