#pragma once

#include <vector>
#include <map>
#include <Python.h>

#include "Player.hpp"
#include "Minmax.hpp"

class Gomoku {

public:

	Gomoku(int size);

	static Gomoku* gomoku;
	std::vector<Player> players;

	static PyObject* init(PyObject* self, PyObject* args);
	
	static PyObject* place(PyObject* self, PyObject* args);
	static PyObject* switchPlayer(PyObject* self, PyObject* args);

	static PyObject* run(PyObject* self, PyObject* args);

	void place(int& y, int& x, int& playerIndex);
	void switchPlayer();

	void printBoard();
	std::string hashState();

	int heuristic();
	int heuristicByPlayer(int player);
	int evalLine(std::pair<int, int> start, std::pair<int, int>& line, int& player);
	std::vector<std::pair<int, int>> getMoves();
	void doMove(std::pair<int, int>& pos);
	void undoMove(std::pair<int, int>& pos);

// private:

	int size;
	std::vector<std::vector<int>> board;
	Player* currentPlayer;
	Player* heuristicPlayer;
	bool isPlaying;
	std::vector<std::pair<int, int>> lastMoves;
	Minmax* minmax;
};