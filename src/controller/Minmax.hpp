#pragma once

#include <Python.h>
#include <map>

class Gomoku;

class Minmax {

public:

	Minmax(Gomoku& gomoku, int maxDepth);
	std::pair<int, int> run();
	std::pair<int, int> bestMove;
	int bestValue;
	std::map<std::string, int> TT;

	int maxDepth;

private:

	Gomoku& gomoku;
	int minmaxAlphaBeta(int depth, int alpha, int beta, int player);
};

#include "Gomoku.hpp"
