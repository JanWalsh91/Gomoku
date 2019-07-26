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
	
	this->minmaxAlphaBeta(this->maxDepth, Minmax::INF_MIN, Minmax::INF_MAX, 1);

	std::cout << "heuristicValues for player " << this->gomoku.currentPlayer->index << std::endl;
	this->gomoku.printBoard(heuristicValues);
	std::cout << std::endl;

	return this->bestMove;
}

int Minmax::minmaxAlphaBeta(int depth, int alpha, int beta, int player) {

	if (depth == 0 || this->gomoku.endState != Gomoku::PLAYING) {
		if (this->gomoku.endState != Gomoku::PLAYING) {
			// int sign = this->gomoku.heuristicPlayer->index == 0 ? -1 : 1;
			int winner = this->gomoku.endState == this->gomoku.heuristicPlayer->index ? 1 : -1;
			return this->scaleByDepth(depth, player * winner * (Minmax::CERTAIN_VICTORY - (this->maxDepth - depth)));
		} else {
			return this->scaleByDepth(depth, player * this->gomoku.heuristic());
		}
	}

	auto moves = this->gomoku.getMoves();

	int value = Minmax::INF_MIN;
	for (auto &move: moves) {

		auto undoMoves = this->gomoku.doMove(move);
		int ret = -this->minmaxAlphaBeta(depth - 1, -beta, -alpha, -player);
		value = std::max(value, ret);
		if (depth == this->maxDepth || this->gomoku.endState != Gomoku::PLAYING) {
			heuristicValues[move.first][move.second] = ret;
		}
		// if (this->bestValue < ret) {
		if ((depth == this->maxDepth || this->gomoku.endState != Gomoku::PLAYING) && this->bestValue < ret) {
			this->bestMove = move;
			this->bestValue = value;
		}
		this->gomoku.undoMove(undoMoves);

		alpha = std::max(alpha, value);
		if (alpha >= beta) {
			break;
		}
	}
	return value;
}

int		Minmax::scaleByDepth(int depth, int value) {
	// return value * (depth + 1);
	return value;
}