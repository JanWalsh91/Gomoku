#include <Python.h>
#include <iostream>
#include <sstream>

#include <iomanip>

#include "Gomoku.hpp"

// TODO: is PLaying, reset function
Gomoku::Gomoku(int size, Player::Type player0Type, Player::Type player1Type): size(size), playing(false), remainingStones(size * size), endState(State::PLAYING), winStreakLength(5) {
	this->players.push_back(Player(0, player0Type));
	this->players.push_back(Player(1, player1Type));

	this->board = std::vector<std::vector<int>>(size, std::vector<int>(size, -1));

	this->currentPlayer = &this->players[0];
	this->heuristicPlayer = nullptr;
	this->lastMoves = std::vector<std::pair<int, int>>(2, std::make_pair<int, int>(-1, -1));
}

void Gomoku::reset() {
	this->board = std::vector<std::vector<int>>(size, std::vector<int>(size, -1));
	this->currentPlayer = &this->players[0];
	this->heuristicPlayer = nullptr;
	this->lastMoves = std::vector<std::pair<int, int>>(2, std::make_pair<int, int>(-1, -1));
	this->endState = State::PLAYING;
	this->remainingStones = this->size * this->size;
	this->playing = false;
}

int Gomoku::checkWinCondition(std::pair<int, int> pos, int& playerIndex) {
	// std::cout << "checkWinCondition" << std::endl;
	// this->printBoard();

	// std::cout << "remainingStones " << remainingStones << std::endl;
	if (this->remainingStones <= 0) {
		return State::DRAW;
	}

	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> lines = {
		std::make_pair(std::make_pair( 0, 1 ), std::make_pair( 0, -1 )),
		std::make_pair(std::make_pair( 1, 0 ), std::make_pair(-1,  0 )),
		std::make_pair(std::make_pair( 1, 1 ), std::make_pair(-1, -1 )),
		std::make_pair(std::make_pair(-1, 1 ), std::make_pair( 1, -1 ))
	};
	
	for (std::pair<std::pair<int, int>, std::pair<int, int>>& line: lines) {
		int numAligned = 1;

			for (int i = 1; i < 5; i++) {
				std::pair<int, int> nextPos = std::make_pair(pos.first + line.first.first * i, pos.second + line.first.second * i);
				if (nextPos.first >= this->size || nextPos.first < 0 ||
					nextPos.second >= this->size || nextPos.second < 0 ||
					this->board[nextPos.first][nextPos.second] != playerIndex) {
						break ;
				} else {
					numAligned++;
					if (numAligned >= 5) {
						// std::cout << "FIVE ALIGNED (1) [" << pos.first << ", " << pos.second << "]" << std::endl;
						return playerIndex;
					}
				}
			}

			for (int i = 1; i < 5; i++) {
				std::pair<int, int> nextPos = std::make_pair(pos.first + line.second.first * i, pos.second + line.second.second * i);
				if (nextPos.first >= this->size || nextPos.first < 0 ||
					nextPos.second >= this->size || nextPos.second < 0 ||
					this->board[nextPos.first][nextPos.second] != playerIndex) {
						break ;
				} else {

					numAligned++;
					if (numAligned >= 5) {
						// std::cout << "FIVE ALIGNED (2) [" << pos.first << ", " << pos.second << "]" << std::endl;
						return playerIndex;
					}
				}
			}
			
	}
	return State::PLAYING;
}

std::vector<AAction*> Gomoku::place(int& y, int& x, int& playerIndex) {
	std::vector<AAction*> actions;


	std::pair<int, int> pos = std::make_pair(y, x);

	this->board[y][x] = playerIndex;
	this->lastMoves[playerIndex] = pos;

	actions.push_back(new ActionUpdateBoard(pos, -1));

	this->remainingStones--;

	int state = this->checkWinCondition(pos, playerIndex);
	
	if (state != State::PLAYING) {
		actions.push_back(new ActionSetEndState(this->endState));
		this->endState = state;
	}
	// TODO: Captures, rules, etc...
	return actions;
}

void Gomoku::place(int& y, int& x) {

	std::pair<int, int> pos = std::make_pair(y, x);

	this->board[y][x] = this->currentPlayer->index;
	this->lastMoves[this->currentPlayer->index] = pos;
	
	this->remainingStones--;
	
	int state = this->checkWinCondition(pos, this->currentPlayer->index);
	if (state != State::PLAYING) {
		this->endState = state;
	}
}

void Gomoku::switchPlayer() {
	// std::cout << "Current Player: " << this->currentPlayer->index << std::endl;
	if (this->currentPlayer == &this->players[0]) {
		this->currentPlayer = &this->players[1];
	} else {
		this->currentPlayer = &this->players[0];
	}
	// std::cout << "New Player: " << this->currentPlayer->index << std::endl;
}

std::string Gomoku::hashState() {
	std::ostringstream oss;
	for (int j = 0; j < this->size; j++) {
		for (int i = 0; i < this->size; i++) {
			oss << this->board[j][i] << ",";
		}
	}
	return oss.str();
	// std::cout << oss.str() << std::endl;
}

std::vector<std::pair<int, int>> Gomoku::getMoves() {
	std::vector<std::pair<int, int>> moves;

	int distAroundLastMoves = 1;

	int currentPlayer = this->currentPlayer->index;

	bool empty = true; 


	// this->printBoard();
	// std::cout << "Last move: [" << move.first << "; " << move.second << "]" << std::endl;
	// move = this->lastMoves[1];


	for (int i = (currentPlayer == 0 ? 0 : 1); i != (currentPlayer == 0 ? 2 : -1); i += (currentPlayer == 0 ? 1 : -1)) {
		auto move = this->lastMoves[i];
		
		if (move.first != -1) {
			for (int i = -distAroundLastMoves; i <= distAroundLastMoves; i++) {
				int i2 = i + move.first;
				for (int j = -distAroundLastMoves; j <= distAroundLastMoves; j++) {
					int j2 = j + move.second;
					if (i2 >= 0 && i2 < this->size && j2 >= 0 && j2 < this->size && this->board[i2][j2] == -1) {
						auto child = std::make_pair(i2, j2);
						if (std::find(moves.begin(), moves.end(), child) == moves.end()) {
							moves.push_back(child);
							empty = false;
						}
					}
				}
			}
		}
	}

	int dist = 2;
	for (int i = 0; i < this->size; i++) {
		for (int j = 0; j < this->size; j++) {
			if (this->board[i][j] != -1) {
				for (int k = -dist; k <= dist; k++) {
					for (int l = -dist; l <= dist; l++) {
						if (i + k >= 0 && i + k < this->size && j + l >= 0 && j + l < this->size && this->board[i + k][j + l] == -1) {
							auto child = std::make_pair(i + k, j + l);
							if (std::find(moves.begin(), moves.end(), child) == moves.end()) {
								moves.push_back(child);
								empty = false;
							}
						}
					}
				}
			}
		}
	}

	if (empty) {
		std::cout <<" EMPTY" << std::endl;
		// this->printBoard();
		moves.push_back(std::make_pair<int, int>((int)(this->size / 2), (int)(this->size / 2)));
	}

	// std::cout << "===== GET CHILD " << std::endl;
	// for (unsigned long i = 0; i < moves.size(); i++) {
	// 	std::cout << moves[i].first << ", " << moves[i].second << std::endl;
	// }
	// std::cout << "===== GET CHILD " << std::endl;

	return moves;
}

const int VICTORY = 1'000;
// const int THREAT  = 1'000'000;

int Gomoku::evalStreakScore(int currentStreakNum, int currentStreakPotential, bool halfOpen, int player, bool discontinuedStreak) {
	std::cout << std::endl << "evalStreakScore. currentStreakNum: " << currentStreakNum << ", currentStreakPotential: " << currentStreakPotential << ", halfOpen: " << halfOpen << ", discontinuedStreak: " << discontinuedStreak <<  std::endl;
	if (currentStreakPotential < 5) {
		return 0;
	}

	if (currentStreakNum >= 5 && !discontinuedStreak) {
		// std::cout << "ON N'EST PAS LA !!!" << std::endl;
		// return VICTORY;
	}

	// return currentStreakNum;

	if (player == this->heuristicPlayer->index) { // ??
		// Look for victory conditions
		if (currentStreakNum >= 5) {
			return VICTORY;
		}
		if (currentStreakNum == 4 && !halfOpen && !discontinuedStreak) {
			return VICTORY;
		}
		std::cout <<  "I IZ HUERE" << std::endl;
		return pow(currentStreakNum, 3) * (halfOpen ? 1 : 2) * (discontinuedStreak ? 1 : 2);  // give particiaption medal
	} else {
		// Look for threats
		if (currentStreakNum >= 5) {
			return VICTORY;
		}
		if (currentStreakNum == 4) {
			return VICTORY;
		}
		if (currentStreakNum == 3 && !halfOpen) {
			return VICTORY;
		}
		return 0;
	}


	// if (discontinuedStreak) {
	// 	currentStreakNum++;	
	// }

	// if (currentStreakNum < 3) {
	// 	if (halfOpen) {
	// 		return currentStreakNum;
	// 	} else {
	// 		return currentStreakNum * 2;
	// 	}
	// }
	// if (currentStreakNum == 3) {
	// 	if (!halfOpen && player == this->currentPlayer->index) {
	// 		return victory;
	// 	}
	// 	return currentStreakNum * 
	// 		(halfOpen ? 1 : 2) *  
	// 		(player == this->currentPlayer->index ? 1 : 2); 
	// }
	// if (currentStreakNum == 4) {
	// 	if (!halfOpen) {
	// 		// CERTAIN VICTORY
	// 		return victory;	
	// 	}
	// 	if (player == this->currentPlayer->index) {
	// 		// CERTAIN VICTORY
	// 		return victory;
	// 	}
	// 	return currentStreakNum;
	// }
	// if (currentStreakNum == 5) {
	// 	// std::cout << "ON EST PAS LA" << std::endl;
	// 	// VICTORY
	// 	return victory * 10;
	// }


	// if (currentStreakNum == 3 && (!halfOpen || player == this->currentPlayer->index)) {
	// 	// score = pow(100, currentStreakNum);
	// 	score = 50;
	// } else if ((currentStreakNum == 4 && (!halfOpen || player == this->currentPlayer->index)) || currentStreakNum == 5) {
	// 	score = 100;
	// } else if (currentStreakNum == 4 && (halfOpen || player == this->currentPlayer->index)) {
	// 	score = 40;
	// } else if (currentStreakNum > 0) {
	// 	// score = pow(currentStreakNum, 3) * (halfOpen ? 1 : 2);
	// 	// score = currentStreakNum * 2 + (halfOpen ? 0 : 5);
	// 	score = halfOpen ? currentStreakNum : currentStreakNum * 2;
	// }


	// std::cout << "evalStreakScore currentStreakNum: " << currentStreakNum << ", currentStreakPotential: " << currentStreakPotential << ", halfOpen: " << halfOpen << std::endl;
	// std::cout << "score: " << score << std::endl;

	// return score;
	return 0;
}

void resetStreak(bool &streaking, bool &frontBlocked, bool &discontinuedStreak, int &currentStreakNum, int &currentStreakPotential, int& emptyCellCount) {
	std::cout << std::endl << "resetStreak" << std::endl;
	streaking = false;
	frontBlocked = false;
	discontinuedStreak = false;
	currentStreakNum = 0;
	currentStreakPotential = 0;
	emptyCellCount = 0;
}

int Gomoku::evalLine(std::pair<int, int> start, std::pair<int, int>& line, int& player, int length) {

	int score = 0;
	bool streaking, frontBlocked, discontinuedStreak;
	int currentStreakNum, currentStreakPotential, emptyCellCount;

	resetStreak(streaking, frontBlocked, discontinuedStreak, currentStreakNum, currentStreakPotential, emptyCellCount);
	// currentStreakNum = 1;
	// currentStreakPotential = 1;

	int otherPlayer = player == 0 ? 1 : 0;

	std::pair<int, int> pos;
	for (int i = 0; i < length; i++) {
		std::cout << "i: " << i << ". ";
		pos = std::make_pair<int, int>(start.first + line.first * i, start.second + line.second * i);

		// if self
		if (this->board[pos.first][pos.second] == player) {
			if (!streaking) {
				if (currentStreakNum) {
					score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked, player, discontinuedStreak);
					
					std::cout << ", score => " << score;
					resetStreak(streaking, frontBlocked, discontinuedStreak, currentStreakNum, currentStreakPotential, emptyCellCount);
					std::cout << ", set streaking to true"; 
					
					currentStreakNum = 1;
					currentStreakPotential = 1;
					std::cout << ", set currentStreakPotential to " << currentStreakPotential; 
				} else {
					currentStreakNum++;
					currentStreakPotential++;
				}
				frontBlocked = pos.second - 1 < 0 || this->board[pos.first - line.first][pos.second - line.second] == otherPlayer;
				std::cout << ", set frontBlocked to " << frontBlocked;
				streaking = true;
			} else {
				if (emptyCellCount == 1) {
					if (!discontinuedStreak) {
						discontinuedStreak = true;
						std::cout << ", set discontinuedStreak to true"; 
					} else {
						score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked, player, discontinuedStreak);
						std::cout << ", score => " << score;
						resetStreak(streaking, frontBlocked, discontinuedStreak, currentStreakNum, currentStreakPotential, emptyCellCount);
						frontBlocked = pos.second - 1 < 0 || this->board[pos.first - line.first][pos.second - line.second] == otherPlayer;
						std::cout << ", set frontBlocked to " << frontBlocked;
						streaking = true;
					}
				}
				currentStreakPotential++;
				std::cout << ", increment currentStreakPotential to " << currentStreakPotential; 
				currentStreakNum++;
			}
			std::cout << ", set emptyCellCount to 0"; 
			emptyCellCount = 0;
		}

		// if empty
		if (this->board[pos.first][pos.second] == -1) {
			currentStreakPotential++;
			std::cout << ", increment currentStreakPotential to " << currentStreakPotential; 
			emptyCellCount++;
			std::cout << ", increment emptyCellCount"; 

			if (streaking) {
				// if only one empty space before
				if (emptyCellCount > 1) {
					streaking = false;
					// discontinuedStreak = true;
					std::cout << ", set streaking to false"; 
					// std::cout << ", set discontinuedStreak to false"; 
				}

			} else {
				// if (frontBlocked && currentStreakPotential == 1) {
				// 	std::cout << ", set frontBlocked to false"; 
				// 	frontBlocked = false;
				// }
			}
		}

		// if other player
		if (this->board[pos.first][pos.second] == otherPlayer) {
			if (streaking) {
				score += this->evalStreakScore(currentStreakNum, currentStreakPotential, true, player, discontinuedStreak);
				std::cout << ", score => " << score;
			} else {
				score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked, player, discontinuedStreak);
				std::cout << ", score => " << score;
			}
			resetStreak(streaking, frontBlocked, discontinuedStreak, currentStreakNum, currentStreakPotential, emptyCellCount);

		}
		std::cout << std::endl;
	}

	// if end
	if (streaking) {
		score += this->evalStreakScore(currentStreakNum, currentStreakPotential, true, player, discontinuedStreak);
	} else {
		score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked, player, discontinuedStreak);
	}

	return score;
}

int Gomoku::heuristicByPlayer(int player) {
	int x = 0;
	int score = 0;
	std::pair<int, int> hLine = std::make_pair<int, int>(0, 1);
	std::pair<int, int> vLine = std::make_pair<int, int>(1, 0);

	std::pair<int, int> dLine1 = std::make_pair<int, int>(1, 1);
	std::pair<int, int> dLine2 = std::make_pair<int, int>(1, -1);

	// std::cout << "heuristicByPlayer for player " << player << std::endl;

	for (int i = 0; i < this->size; i++) {
		int x = this->evalLine(std::make_pair(i, 0), hLine, player, this->size);
		// std::cout << "adding " << x << " to player score " << player << " at line (" << i << ", 0)" << " => " << score << std::endl; 
		score += x;
		// std::cout << "score => " << score << std::endl;
		x = this->evalLine(std::make_pair(0, i), vLine, player, this->size);
		// std::cout << "adding " << x << " to player score " << player << " at line (0, " << i << ")" << " => " << score << std::endl; 
		score += x;
		// std::cout << "score => " << score << std::endl;
	}

	for (int i = 0; i < this->size - this->winStreakLength; i++) {
		x = this->evalLine(std::make_pair(0, i), dLine1, player, this->size - i);
		// std::cout << "adding " << x << " to player " << player << " at line (0, " << i << ")" << ", direction: (" << dLine1.first << ", " << dLine1.second << ") SCORE:[" << score << " => ";
		score += x;
		// std::cout << score << "]" << std::endl;
		if (i != 0) {
			x = this->evalLine(std::make_pair(i, 0), dLine1, player, this->size - i);
			// std::cout << "adding " << x << " to player " << player << " at line (" << i << ", 0)" << ", direction: (" << dLine1.first << ", " << dLine1.second << ") SCORE:[" << score << " => ";
			score += x;
			// std::cout << score << "]" << std::endl; 
		}
	}

	for (int i = this->size - 1; i >= this->winStreakLength; i--) {

		x = this->evalLine(std::make_pair(0, i), dLine2, player, i + 1);
		// std::cout << "adding " << x << " to player " << player << " at line (0, " << i << ")" << ", direction: (" << dLine2.first << ", " << dLine2.second << ") SCORE:[" << score << " => ";
		score += x;
		// std::cout << score << "]" << std::endl;
	}

	for (int i = 1; i <= this->size - this->winStreakLength; i++) {
		x = this->evalLine(std::make_pair(i, this->size - 1), dLine2, player, this->size - i);

		// std::cout << "adding " << x << " to player " << player << " at line (" << i << ", " << this->size - 1 <<  ")" << ", direction: (" << dLine2.first << ", " << dLine2.second << ") SCORE:[" << score << " => ";
		score += x;
		// std::cout << score << "]" << std::endl;
	}

	return score;
}

float hPlayerMultiplier = 1;

int Gomoku::heuristic(int depth) {

	// std::cout << "HEU" << std::endl;
	// this->printBoard();
	int score0 = this->heuristicByPlayer(0);
	int score1 = this->heuristicByPlayer(1);
	// std::cout << std::endl << "score0: " << score0 << ", score1: " << score1 << std::endl;
	// std::cout << std::endl;
	// return score1 - (int)(score0);

	// std::cout << "INDEX: " << this->heuristicPlayer->index << std::endl;

	// std::cout << "Before factors:" << std::endl;
	// std::cout << "Score0: " << score0 << std::endl;
	// std::cout << "Score1: " << score1 << std::endl;

	// std::cout << "finalScore before depth: " << (this->heuristicPlayer->index == 0 ? score0 - score1 : score1 - score0) << std::endl;
	// return this->heuristicByPlayer(this->heuristicPlayer->index);
	// int finalScore = (this->heuristicPlayer->index == 0 ? hPlayerMultiplier * score0 - score1 :  hPlayerMultiplier * score1 - score0) / (this->minmax->maxDepth - depth);

	int finalScore = this->heuristicPlayer->index == 0 ? hPlayerMultiplier * score0 - score1 :  hPlayerMultiplier * score1 - score0;
	// if (depth != 0) {
		// std::cout << "depth: " << depth << ", finalScore: " << finalScore << " => "; 
		finalScore *= (1 + (depth / (float)this->minmax->searchDepth));
		// finalScore *= (depth + 1);
		// std::cout << finalScore << std::endl;
	// }
	// std::cout << "finalScore: " << finalScore << std::endl;

	return finalScore;
	// return this->heuristicByPlayer(this->heuristicPlayer->index) - this->heuristicByPlayer(this->heuristicPlayer->index == 1 ? 0 : 1) * 2.1f;
}


std::vector<AAction*> Gomoku::doMove(std::pair<int, int>& pos) {

	std::vector<AAction*> actions = this->place(pos.first, pos.second, this->currentPlayer->index);
	this->switchPlayer();
	return actions;
}

void Gomoku::undoMove(std::vector<AAction*>& actions) {
	this->switchPlayer();
	

	ActionUpdateBoard* aub;
	ActionSetEndState* aes;
	for (AAction* action: actions) {
		switch(action->type) {
			case AAction::Type::UPDATE_BOARD:
				aub = dynamic_cast<ActionUpdateBoard*>(action);
				this->board[aub->pos.first][aub->pos.second] = aub->value;
				this->remainingStones += aub->value >= 0 ? -1 : 1;
				break;
			case AAction::Type::SET_END_STATE:
				aes = dynamic_cast<ActionSetEndState*>(action);
				this->endState = aes->state;
				break;
		}

		delete action;
	}
}

void Gomoku::printBoard() {
	for (int j = 0; j < this->size; j++) {
		for (int i = 0; i < this->size; i++) {
			std::cout << std::setw(6) << this->board[j][i] << " ";
		}
		std::cout << std::endl;
	}
}

void Gomoku::printBoard(std::vector<std::vector<int>> board) {
	for (int j = 0; j < this->size; j++) {
		for (int i = 0; i < this->size; i++) {
			std::cout << std::setw(6) << board[j][i] << " ";
		}
		std::cout << std::endl;
	}
}

Gomoku* Gomoku::gomoku = nullptr;

PyObject* Gomoku::init(PyObject* self, PyObject* args) {
	int size;
	int maxDepth;
	int player0Type;
	int player1Type;

	if (!PyArg_ParseTuple(args, "iiii", &size, &maxDepth, &player0Type, &player1Type)) {
		return NULL;
	}

	Gomoku::gomoku = new Gomoku(size, (Player::Type)player0Type, (Player::Type)player1Type);
	Gomoku::gomoku->minmax = new Minmax(*Gomoku::gomoku, maxDepth);

	return PyLong_FromLong(0);
}

PyObject* Gomoku::reset(PyObject* self, PyObject* args) {
	Gomoku::gomoku->reset();
	return PyLong_FromLong(0);
}

PyObject* Gomoku::run(PyObject* self, PyObject* args) {
	// std::cout << "Gomoku Run" << std::endl;

	auto pos = Gomoku::gomoku->minmax->run();

	// std::cout << "pos: " << pos.first << ", " << pos.second << std::endl;

	PyObject* res = PyTuple_New(2);
    PyObject* y = PyLong_FromLong(pos.first);
    PyObject* x = PyLong_FromLong(pos.second);
    PyTuple_SetItem(res, 0, y);
    PyTuple_SetItem(res, 1, x);

	return res;
}

PyObject* Gomoku::place(PyObject* self, PyObject* args) {
	int y, x;
	
	if (!PyArg_ParseTuple(args, "ii", &y, &x)) {
		return NULL;
	}
	std::cout << "=> Python choose to place at " << y << ", " << x << std::endl;

	Gomoku::gomoku->place(y, x);
	return PyLong_FromLong(0);
}

PyObject* Gomoku::switchPlayer(PyObject* self, PyObject* args) {
	Gomoku::gomoku->switchPlayer();
	return PyLong_FromLong(0);
}

PyObject* Gomoku::getEndState(PyObject* self, PyObject* args) {
	return PyLong_FromLong(Gomoku::gomoku->endState);
}

PyObject* Gomoku::isPlaying(PyObject* self, PyObject* args) {
	return PyBool_FromLong(Gomoku::gomoku->playing);
}


PyObject* Gomoku::setPlaying(PyObject* self, PyObject* args) {
	// bool playing;
	
	if (!PyArg_ParseTuple(args, "b", &Gomoku::gomoku->playing)) {
		return NULL;
	}

	// Gomoku::gomoku->playing = playing;
	return PyLong_FromLong(0);
}

PyObject* Gomoku::isCurrentPlayerAI(PyObject* self, PyObject* args) {
	return PyBool_FromLong(Gomoku::gomoku->currentPlayer->type == Player::AI);
}


PyObject* Gomoku::setPlayerType(PyObject* self, PyObject* args) {
	int playerIndex;
	int playerType;
	

	if (!PyArg_ParseTuple(args, "ii", &playerIndex, &playerType)) {
		return NULL;
	}
	std::cout << "setPlayerType to: " << playerType << std::endl;

	Gomoku::gomoku->players[playerIndex].type = (Player::Type)playerType;
	return PyLong_FromLong(0);
}


static PyMethodDef methods[] = {
	{"init", Gomoku::init, METH_VARARGS, "Returns the heuristic value."},
	{"reset", Gomoku::reset, METH_VARARGS, "Returns the reset value."},
	{"place", Gomoku::place, METH_VARARGS, "Returns the place value."},
	{"switch_player", Gomoku::switchPlayer, METH_VARARGS, "Returns the switchPlayer value."},
	{"run", Gomoku::run, METH_VARARGS, "Returns the minmax value."},
	{"get_end_state", Gomoku::getEndState, METH_VARARGS, "Returns the minmax value."},
	{"is_playing", Gomoku::isPlaying, METH_VARARGS, "Returns the playing value."},
	{"set_playing", Gomoku::setPlaying, METH_VARARGS, "Returns the playing value."},
	{"is_current_player_AI", Gomoku::isCurrentPlayerAI, METH_VARARGS, "Returns the ai state machine."},
	{"set_player_type", Gomoku::setPlayerType, METH_VARARGS, "Returns the ai pouet machine."},
	{"test_eval_line", Gomoku::testEvalLine, METH_VARARGS, "Returns the eval_line_test pouet machine."},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef definition = {
	PyModuleDef_HEAD_INIT,
	"GomokuModule",
	"A Python module containing Classy type and pants() function",
	-1,
	methods
};

PyMODINIT_FUNC PyInit_GomokuModule(void) {
	Py_Initialize();
	PyObject *m = PyModule_Create(&definition);

	return m;
}


void printLineScore(std::vector<int> line, int size, int score) {
	std::cout << "==============";
	for (int i = 0; i < size; i ++) {
		std::cout << line[i] << ", ";
	}
	std::cout << " => score: " << score << std::endl << std::endl;
}

PyObject* Gomoku::testEvalLine(PyObject* self, PyObject* args) {
	std::cout << "testEvalLine" << std::endl;
	Gomoku::gomoku = new Gomoku(10, Player::Type::AI, Player::Type::AI);

	Gomoku::gomoku->board = std::vector<std::vector<int>>();

	std::pair<int, int> dir = std::make_pair(0, 1);

	int player = 0;
	Gomoku::gomoku->heuristicPlayer = &Gomoku::gomoku->players[0];
	int lines[][10] = {
		{ 0, 0, 0, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, 0, 0, -1, -1, -1 },
		{ 0, 0, 0, -1, -1, 0, 0, -1, -1, -1 },
		{ -1, 0, 0, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, 0, 0, -1, -1, -1, -1, -1, -1, -1 },
		{ 1, 0, 0, 1, -1, -1, -1, -1, -1, -1 },
		{ -1, 0, 0, -1, 0, -1, -1, -1, -1, -1 },
		{ -1, 0, 0, -1, 0, 0, -1, -1, -1, -1 },
		{ -1, 0, 0, -1, -1, 0, 0, -1, -1, -1 },
		{ -1, 0, 0, -1, 0, -1, -1, -1, -1, -1 },
	};
	for (auto &line: lines) {
		Gomoku::gomoku->board.push_back(std::vector<int> (std::begin(line), std::end(line)));
	}
	Gomoku::gomoku->printBoard();

	for (int i = (int)((1 - 1) * 2 * 0.5); i < 10; i++) {
		int ret = Gomoku::gomoku->evalLine(std::make_pair(i, 0), dir, player, 10);
		printLineScore(Gomoku::gomoku->board[i], 10, ret);
	}
	return PyLong_FromLong(0);

}
