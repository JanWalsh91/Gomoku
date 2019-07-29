#pragma once

#include <vector>
#include <map>
//#include <Python.h>
#include <memory>

#include "Player.hpp"
#include "Minmax.hpp"
#include "AAction.hpp"
#include "ActionUpdateBoard.hpp"
#include "ActionSetEndState.hpp"
#include "rules/ARule.hpp"

class ARule;

class Gomoku {

public:

	enum State {
		DRAW = -2,
		PLAYING = -1
	};

	Gomoku(int size, Player::Type player0Type, Player::Type player1Type);

	static Gomoku* gomoku;
	std::vector<std::shared_ptr<Player>> players;

	//static PyObject* init(PyObject* self, PyObject* args);
	//static PyObject* reset(PyObject* self, PyObject* args);
	//
	//static PyObject* place(PyObject* self, PyObject* args);
	//static PyObject* switchPlayer(PyObject* self, PyObject* args);

	//static PyObject* run(PyObject* self, PyObject* args);
	//static PyObject* getEndState(PyObject* self, PyObject* args);

	//static PyObject* isPlaying(PyObject* self, PyObject* args);
	//static PyObject* setPlaying(PyObject* self, PyObject* args);

	//static PyObject* isCurrentPlayerAI(PyObject* self, PyObject* args);
	//static PyObject* setPlayerType(PyObject* self, PyObject* args);

	//static PyObject* undo(PyObject* self, PyObject* args);


	//static PyObject* testEvalLine(PyObject* self, PyObject* args);
	//static PyObject* canPlace(PyObject* self, PyObject* args);


	std::vector<AAction*> place(int& y, int& x, int& playerIndex);
	//void place(int& y, int& x);
	void switchPlayer();
	void reset();

	void printBoard();
	void printBoard(std::vector<std::vector<int>> board);

	int checkWinCondition(std::pair<int, int> pos, int& playerIndex);

	int heuristic();
	int heuristicByPlayer(int player);
	int evalLine(std::pair<int, int> start, std::pair<int, int> line, int player, int length);
	std::vector<std::pair<int, int>> getMoves();
	std::vector<AAction*> doMove(std::pair<int, int>& pos);
	void undoMove(std::vector<AAction*>& actions);

	bool canPlace(std::pair<int, int>) const;

// private:

	std::vector<ARule*> rules;

	int evalStreakScore(int, int, bool, int player, bool discontinuedStreak);

	std::vector<std::vector<AAction*>> stackActions;

	int size;
	std::vector<std::vector<int>> board;
	std::shared_ptr<Player> currentPlayer;
	std::shared_ptr<Player> heuristicPlayer;
	bool playing;
	std::vector<std::pair<int, int>> lastMoves;
	std::shared_ptr<Minmax> minmax;
	int remainingStones;
	int endState;
	int winStreakLength;
};