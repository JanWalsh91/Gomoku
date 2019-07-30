#pragma once

#include "Common.hpp"

class ARule;
class Minmax;

class Gomoku {

public:

	enum State {
		DRAW = -2,
		PLAYING = -1
	};

	Gomoku(int size, Player::Type player0Type, Player::Type player1Type);

	static Gomoku* gomoku;
	std::vector<std::shared_ptr<Player>> players;

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

	void nextTurn();
	int getTurn() const;
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

	int turn;
};