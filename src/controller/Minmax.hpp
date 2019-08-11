#pragma once

#include "Common.hpp"

class Gomoku;

class Minmax {

public:

	enum EntryFlag {
		Upperbound,
		Lowerbound,
		Exact
	};

	typedef struct {
		std::pair<int, int> move;
		int heuristic;
	} HeuristicByMove;

	typedef struct {
		int value;
		EntryFlag flag;
		int depth;
	} TableEntry;

	Minmax(Gomoku& gomoku, int maxDepth);
	std::pair<int, int> run();

	void setMaxDepth(int depth);

	bool isRunning() const;
	int	 getMaxDepth() const;


	// Transposition Tables
	TableEntry*	getTTEntry(std::vector<char> hash);
	void		addTTEntry(std::vector<char> hash, char value, EntryFlag flag, int depth);

	static const int LOSS;
	static const int VICTORY;
	static const int CERTAIN_VICTORY;
	static const int PROXIMITY_BONUNS;

private:

	int _minmaxAlphaBeta(int depth, int alpha, int beta, bool maximizing, bool root, int heuristicValue);
	std::vector<HeuristicByMove> _getSortedMoves(std::vector<std::pair<int, int>>& moves, bool maximizing, int depth);

	std::vector<std::vector<int>> _heuristicValues;
	std::map<std::vector<char>, TableEntry> _TT;
	Gomoku& _gomoku;
	bool _running;
	std::pair<int, int> _bestMove;
	int _bestValue;
	int _maxDepth;
};

#include "Gomoku.hpp"
