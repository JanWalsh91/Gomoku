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
	std::map<std::string, int> TT; // heuristic values for game state // Transposition Table
	// 
	std::map<std::string,	// nodeKey 
		std::vector<		// list of
			std::pair<		// pair of
				std::pair<int, int>,	// position
				int>>> IDTable;			// value

	/*
		{
			nodeKey: [
				[
					childNodeKey,
					childNodeVal
				]
				...
			]
			...
		}
	*/


	int maxDepth;
	int searchDepth;

private:

	Gomoku& gomoku;
	int minmaxAlphaBeta(int depth, int alpha, int beta, int player);
};

#include "Gomoku.hpp"