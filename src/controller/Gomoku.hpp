#pragma once

#include "Common.hpp"

class ARule;
class Minmax;
class AAction;
class GameEndingCapture;

class Gomoku {

public:

	enum State {
		DRAW = -2,
		PLAYING = -1
	};

	Gomoku(int size, Player::Type player0Type, Player::Type player1Type);

	std::vector<std::shared_ptr<Player>> players;
	std::shared_ptr<Player> currentPlayer;
	std::shared_ptr<Player> heuristicPlayer;

	// Game Actions
	void reset();
	void clearReset();
	void nextTurn();
	void switchPlayer();

	// Callbacks
	void onUpdateBoard(std::function<void(std::pair<int, int>, int)> f);
	void onCapture(std::function<void(int playerIndex, int value)> f);
	void updateBoardCallbacks(std::pair<int, int> pos, int value);
	void captureCallbacks(int playerIndex);
	
	// Board 
	bool canPlace(std::pair<int, int>) const;
	bool canPlace(std::pair<int, int>, int playerIndex) const;
	bool onBoard(std::pair<int, int> &pos) const;
	bool notOnBoard(std::pair<int, int> &pos) const;
	
	void place(int y, int x);
	std::vector<AAction*> place(int y, int x, int playerIndex);
	
	std::vector<AAction*> doMove(std::pair<int, int>& pos);
	void undoMove(std::vector<AAction*>& actions);

	int checkWinCondition(std::pair<int, int> pos, int playerIndex);
	int canBreakAlignment(std::pair<int, int> startPos, int playerIndex, int otherPlayer, std::pair<int, int>& dir, int numAligned) const;
	bool canBreakAtPos(std::pair<int, int> nextPos, int playerIndex, int otherPlayer) const;

	void updatePotentialCaptures();
	void updatePotentialCapturesByLine(std::pair<int, int> startPos, std::pair<int, int> dir, int length);

	void setUpStartConfiguration(int i);

	// Heuristic
	int heuristic(int depth);
	int heuristicByPlayer(int player, int *certainVictory);
	int evalLine(std::pair<int, int> start, std::pair<int, int> line, int player, int length, int* certainVictory);
	int evalStreakScore(int, int, bool, int player, int emptyCellCount, int* certainVictory);
	bool hasEnoughPotential(std::pair<int, int> start, std::pair<int, int> line, int countToCheck, int otherPlayer);
	std::vector<std::pair<int, int>> getMoves();
	std::vector<char> hashState();

	// Getters and Setters
	int getTurn() const;

	void setEndState(int i);
	void setEndState(State i);
	State getEndState();

	int getValueOnBoard(std::pair<int, int> pos) const;
	char getValueOnBoard(int y, int x) const;
	void setValueOnBoard(std::pair<int, int> pos, char value);
	void setValueOnBoard(int y, int x, char value);

	int getRemainingStones();
	void setRemainingStones(int i);

	bool hasBeenReset() const;
	
	// Debug
	void printBoard() const;
	void printBoard(std::vector<std::vector<int>> board) const;
	void printBoard(std::vector<std::vector<int>> board, std::pair<int, int> pos) const;

	std::vector<std::shared_ptr<ARule>> _rules;
	std::shared_ptr<GameEndingCapture> _gameEndingCapture;

	int size;
	bool playing;
	std::vector<std::pair<int, int>> lastMoves;
	std::shared_ptr<Minmax> minmax;
	int remainingStones;
	int winStreakLength;

	int turn;
	bool _hasBeenReset;

	std::vector<std::function<void(std::pair<int, int>, int)>> _updateBoardCallbacks;
	std::vector<std::function<void(int playerIndex, int value)>> _captureCallbacks;

	static const int PotentialCapturePattern[][2][4];

private:
	std::vector<char>	_board;
	State				_endState;

	static const std::pair<int, int> hLine;
	static const std::pair<int, int> vLine;
	static const std::pair<int, int> dLine1;
	static const std::pair<int, int> dLine2;
	static const std::vector<std::vector<std::pair<std::pair<int, int>, int>>> startConfigurations;
};

