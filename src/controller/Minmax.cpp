#include <Python.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "Minmax.hpp"

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

	if (maximizing) {

		int m = Minmax::INF_MAX;

		auto moves = this->gomoku.getMoves();
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
				break;
			}
		}
		return m;
	} else {

		int M = Minmax::INF_MIN;

		auto moves = this->gomoku.getMoves();
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
				break;
			}
		}
		return M;
	}

}

int		Minmax::scaleByDepth(int depth, int value) {
	// return value * (depth + 1);
	return value;
}