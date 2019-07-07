#pragma once

#include <vector>
#include <map>
#include <Python.h>

#include "Player.hpp"
#include "Minmax.hpp"
#include "AAction.hpp"
#include "ActionUpdateBoard.hpp"
#include "ActionSetEndState.hpp"

class Gomoku {

public:

	enum State {
		DRAW = -2,
		PLAYING = -1
	};

	Gomoku(int size, Player::Type player0Type, Player::Type player1Type);

	static Gomoku* gomoku;
	std::vector<Player> players;

	static PyObject* init(PyObject* self, PyObject* args);
	static PyObject* reset(PyObject* self, PyObject* args);
	
	static PyObject* place(PyObject* self, PyObject* args);
	static PyObject* switchPlayer(PyObject* self, PyObject* args);

	static PyObject* run(PyObject* self, PyObject* args);
	static PyObject* getEndState(PyObject* self, PyObject* args);

	static PyObject* isPlaying(PyObject* self, PyObject* args);
	static PyObject* setPlaying(PyObject* self, PyObject* args);

	static PyObject* isCurrentPlayerAI(PyObject* self, PyObject* args);
	static PyObject* setPlayerType(PyObject* self, PyObject* args);


	std::vector<AAction*> place(int& y, int& x, int& playerIndex);
	void place(int& y, int& x);
	void switchPlayer();
	void reset();

	void printBoard();
	void printBoard(std::vector<std::vector<int>> board);
	std::string hashState();

	int checkWinCondition(std::pair<int, int> pos, int& playerIndex);

	int heuristic(int depth);
	int heuristicByPlayer(int player);
	int evalLine(std::pair<int, int> start, std::pair<int, int>& line, int& player, int length);
	std::vector<std::pair<int, int>> getMoves();
	std::vector<AAction*> doMove(std::pair<int, int>& pos);
	void undoMove(std::vector<AAction*>& actions);

// private:


	int evalStreakScore(int, int, bool, int player, bool discontinuedStreak);

	int size;
	std::vector<std::vector<int>> board;
	Player* currentPlayer;
	Player* heuristicPlayer;
	bool playing;
	std::vector<std::pair<int, int>> lastMoves;
	Minmax* minmax;
	int remainingStones;
	int endState;
	int winStreakLength;
};