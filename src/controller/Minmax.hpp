#pragma once

#include <Python.h>
#include <map>
#include <algorithm>

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

	static const int INF_MIN = std::numeric_limits<int>::min() + 1;
	static const int INF_MAX = std::numeric_limits<int>::max();

	static const int VICTORY = 1'000;
	static const int CERTAIN_VICTORY = 10'000;
	std::map<std::string, Entry>TT; 

	int maxDepth;

private:

	Gomoku& gomoku;
	int minmaxAlphaBeta(int depth, int alpha, int beta, int player);

	int scaleByDepth(int depth, int value);


};

#include "Gomoku.hpp"