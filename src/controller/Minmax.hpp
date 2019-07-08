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

	struct Entry {
		int value;
		int depth;
		int flag;
	};

	static const int LOWERBOUND = 0;
	static const int UPPERBOUND = 1;
	static const int EXACT      = 2;

	std::map<std::string, Entry>TT; 

	int maxDepth;
	int searchDepth;

private:

	Gomoku& gomoku;
	int minmaxAlphaBeta(int depth, int alpha, int beta, int player);

	int scaleByDepth(int depth, int value);

	static const int VICTORY = 1'000;

};

#include "Gomoku.hpp"