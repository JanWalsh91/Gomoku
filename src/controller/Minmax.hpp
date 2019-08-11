#pragma once

#include "Common.hpp"

class Gomoku;

class Minmax {

public:

	typedef struct {
		std::pair<int, int> move;
		int heuristic;
	} HeuristicByMove;

	enum EntryFlag {
		Upperbound,
		Lowerbound,
		Exact
	};

	typedef struct {
		int value;
		EntryFlag flag;
		int depth;
	} TableEntry;

	Minmax(Gomoku& gomoku, int maxDepth);
	
	std::pair<int, int> run();
	
	std::pair<int, int> bestMove;
	int bestValue;

	int maxDepth;

	std::vector<std::vector<int>> heuristicValues;

	bool isRunning() const;

	// Transposition Tables

	TableEntry*	getTTEntry(std::vector<int> hash);
	void		addTTEntry(std::vector<int> hash, int value, EntryFlag flag, int depth);

	static const int INF_MIN = -100'000'000;
	static const int INF_MAX = 100'000'000;
	static const int PROXIMITY_BONUNS = 10;
	static const int CERTAIN_VICTORY = 10'000;

private:

	std::map<std::vector<int>, TableEntry> _TT;

	Gomoku& gomoku;

	int minmaxAlphaBeta(int depth, int alpha, int beta, bool maximizing, bool root, int heuristicValue);

	std::vector<HeuristicByMove> getSortedMoves(std::vector<std::pair<int, int>>& moves, bool maximizing, int depth);
	
	bool _running;


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

