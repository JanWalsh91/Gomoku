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

	int minmaxAlphaBeta(int depth, int alpha, int beta, bool maximizing);

	int scaleByDepth(int depth, int value);

	std::vector<std::pair<int, int>> getSortedMoves(std::vector<std::pair<int, int>>&, bool maximizing, int depth) const;

};

#include "Gomoku.hpp"

/*
-s19 -d2
without sorting
	0.1661, 0.166 without break
	0.167, 0.1677 with break
with sorting 
	all results
		0.2867, 0.2844 without break
		0.2842, 0.2869 with break
	5th first
		0.1609, 0.1402 without break
		0.1374 with break

-s7 -d4
without sorting
	1.7668 without break
	0.1774 with break
with sorting 
	all results
		3.2179 without break
		0.5739 with break
	5 first (free three)
		0.0934 without break
		0.0542 with break


=> 
- depth > 2 : alpha beta pruning is useful
- limiting nodes to search is better than all
- sorting only userful when depth > 2 and limit nodes

*/

