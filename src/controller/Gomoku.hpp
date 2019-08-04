#pragma once

#include "Common.hpp"

class ARule;
class Minmax;
class AAction;

class Gomoku {

public:

	enum State {
		DRAW = -2,
		PLAYING = -1
	};

	Gomoku(int size, Player::Type player0Type, Player::Type player1Type);

	std::vector<std::shared_ptr<Player>> players;

	static void testEvalLine();
	static void testHeuristic();
	static void testMinmax();

	std::vector<AAction*> place(int y, int x, int playerIndex);
	void place(int y, int x);
	void switchPlayer();
	void reset();

	void printBoard();
	void printBoard(std::vector<std::vector<int>> board);
	void printBoard(std::vector<std::vector<int>> board, std::pair<int, int> pos);
	void printState();

	void updateBoardCallbacks(std::pair<int, int> pos, int value);
	void captureCallbacks(int playerIndex);

	int checkWinCondition(std::pair<int, int> pos, int playerIndex);

	int heuristic();
	int heuristicByPlayer(int player);
	int evalLine(std::pair<int, int> start, std::pair<int, int> line, int player, int length, int &potentialCaptures);
	int evalStreakScore(int, int, bool, int player, int emptyCellCount);
	bool hasEnoughPotential(std::pair<int, int> start, std::pair<int, int> line, int countToCheck, int otherPlayer);
	std::vector<std::pair<int, int>> getMoves();
	std::vector<AAction*> doMove(std::pair<int, int>& pos);
	void undoMove(std::vector<AAction*>& actions);

	bool canPlace(std::pair<int, int>) const;
	bool hasBeenReset() const;
	void clearReset();

	void nextTurn();
	int getTurn() const;

	void onUpdateBoard(std::function<void(std::pair<int, int>, int)> f);
	void onCapture(std::function<void(int playerIndex, int value)> f);

// private:

	std::vector<ARule*> rules;

	// std::vector<std::vector<AAction*>> stackActions;

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
	bool _hasBeenReset;

	std::vector<std::function<void(std::pair<int, int>, int)>> _updateBoardCallbacks;
	std::vector<std::function<void(int playerIndex, int value)>> _captureCallbacks;

	static const int PotentialCapturePattern[][2][4];

};

