#include <Python.h>
#include <iostream>
#include <sstream>

#include <iomanip>

#include "Gomoku.hpp"
#include "rules/NoDoubleFreeThree.hpp"

Gomoku::Gomoku(int size, Player::Type player0Type, Player::Type player1Type): size(size), playing(false), remainingStones(size * size), endState(State::PLAYING), winStreakLength(5) {
	this->players.push_back(Player(0, player0Type));
	this->players.push_back(Player(1, player1Type));

	this->board = std::vector<std::vector<int>>(size, std::vector<int>(size, -1));

	this->currentPlayer = &this->players[0];
	this->heuristicPlayer = nullptr;
	this->lastMoves = std::vector<std::pair<int, int>>(2, std::make_pair<int, int>(-1, -1));

	this->rules.push_back(new NoDoubleFreeThree());
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

void Gomoku::switchPlayer() {
	if (this->currentPlayer == &this->players[0]) {
		this->currentPlayer = &this->players[1];
	} else {
		this->currentPlayer = &this->players[0];
	}
}

std::vector<std::pair<int, int>> Gomoku::getMoves() {
	std::vector<std::pair<int, int>> moves;

	int distAroundLastMoves = 1;

	int currentPlayer = this->currentPlayer->index;

	bool empty = true; 

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
		moves.push_back(std::make_pair<int, int>((int)(this->size / 2), (int)(this->size / 2)));
	}

	auto it = moves.begin();
	while (it != moves.end()) {
		bool erased = false;
		if (!this->canPlace(*it)) {
			erased = true;
			it = moves.erase(it);
		}
		if (!erased) {
			it++;
		}
	}
	return moves;
}

int Gomoku::evalStreakScore(int currentStreakNum, int currentStreakPotential, bool halfOpen, int player, bool discontinuedStreak) {
	if (currentStreakPotential < 5) {
		return 0;
	}

	if (player == this->heuristicPlayer->index) {
		// Look for Minmax::victory conditions
		if (currentStreakNum >= 5 && !discontinuedStreak) {
			std::cout << "SHOULD NOT BE HERE" << std::endl;
			return Minmax::CERTAIN_VICTORY;
		}
		if (currentStreakNum == 4 && !halfOpen && !discontinuedStreak) {
			return Minmax::VICTORY;
		}
		return static_cast<int>(pow(currentStreakNum, 3) * (halfOpen ? 1 : 2) * (discontinuedStreak ? 1 : 2));  // give particiaption medal
	} else {
		// Look for threats
		if (currentStreakNum >= 5) {
			return Minmax::CERTAIN_VICTORY;
		}
		if (currentStreakNum == 4) {
			return Minmax::VICTORY;
		}
		if (currentStreakNum == 3 && !halfOpen) {
			return Minmax::VICTORY;
		}
		return 0;
	}

	return 0;
}

void resetStreak(bool &streaking, bool &frontBlocked, bool &discontinuedStreak, int &currentStreakNum, int &currentStreakPotential, int& emptyCellCount) {
	streaking = false;
	frontBlocked = false;
	discontinuedStreak = false;
	currentStreakNum = 0;
	currentStreakPotential = 0;
	emptyCellCount = 0;
}

int Gomoku::evalLine(std::pair<int, int> start, std::pair<int, int> line, int player, int length) {

	int score = 0;
	bool streaking, frontBlocked, discontinuedStreak;
	int currentStreakNum, currentStreakPotential, emptyCellCount;

	resetStreak(streaking, frontBlocked, discontinuedStreak, currentStreakNum, currentStreakPotential, emptyCellCount);

	int otherPlayer = player == 0 ? 1 : 0;

	std::pair<int, int> pos;

	for (int i = 0; i < length; i++) {
		pos = std::make_pair<int, int>(start.first + line.first * i, start.second + line.second * i);

		// if player
		if (this->board[pos.first][pos.second] == player) {
			if (!streaking) {
				if (currentStreakNum) {
					score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked, player, discontinuedStreak);
					resetStreak(streaking, frontBlocked, discontinuedStreak, currentStreakNum, currentStreakPotential, emptyCellCount);
					currentStreakNum = 1;
					currentStreakPotential = 1;
				} else {
					currentStreakNum++;
					currentStreakPotential++;
				}

				frontBlocked = pos.second - 1 < 0 || pos.first - line.first < 0 || pos.second - line.second < 0 || this->board[pos.first - line.first][pos.second - line.second] == otherPlayer;
				streaking = true;
			} else {
				if (emptyCellCount == 1) {
					if (!discontinuedStreak) {
						discontinuedStreak = true;
					} else {
						score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked, player, discontinuedStreak);
						resetStreak(streaking, frontBlocked, discontinuedStreak, currentStreakNum, currentStreakPotential, emptyCellCount);
						frontBlocked = pos.second - 1 < 0 || pos.first - line.first < 0 || pos.second - line.second < 0 || this->board[pos.first - line.first][pos.second - line.second] == otherPlayer;
						streaking = true;
					}
				}
				currentStreakPotential++;
				currentStreakNum++;
			}
			emptyCellCount = 0;
		}
		if (this->board[pos.first][pos.second] == -1) {
			currentStreakPotential++;
			emptyCellCount++;

			if (streaking) {
				if (emptyCellCount > 1) {
					streaking = false;
				}
			}
		}

		// if other player
		if (this->board[pos.first][pos.second] == otherPlayer) {
			if (streaking) {
				score += this->evalStreakScore(currentStreakNum, currentStreakPotential, true, player, discontinuedStreak);
			} else {
				score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked, player, discontinuedStreak);
			}
			resetStreak(streaking, frontBlocked, discontinuedStreak, currentStreakNum, currentStreakPotential, emptyCellCount);

		}
	}

	if (streaking) {
		score += this->evalStreakScore(currentStreakNum, currentStreakPotential, true, player, discontinuedStreak);
	} else {
		score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked, player, discontinuedStreak);
	}

	return score;
}

int Gomoku::heuristicByPlayer(int player) {
	int score = 0;
	std::pair<int, int> hLine = std::make_pair<int, int>(0, 1);
	std::pair<int, int> vLine = std::make_pair<int, int>(1, 0);

	std::pair<int, int> dLine1 = std::make_pair<int, int>(1, 1);
	std::pair<int, int> dLine2 = std::make_pair<int, int>(1, -1);

	for (int i = 0; i < this->size; i++) {
		score += this->evalLine(std::make_pair(i, 0), hLine, player, this->size);
		score += this->evalLine(std::make_pair(0, i), vLine, player, this->size);
	}

	for (int i = 0; i < this->size - this->winStreakLength; i++) {
		score += this->evalLine(std::make_pair(0, i), dLine1, player, this->size - i);
		if (i != 0) {
			score += this->evalLine(std::make_pair(i, 0), dLine1, player, this->size - i);
		}
	}

	for (int i = this->size - 1; i >= this->winStreakLength; i--) {
		score += this->evalLine(std::make_pair(0, i), dLine2, player, i + 1);
	}

	for (int i = 1; i <= this->size - this->winStreakLength; i++) {
		score += this->evalLine(std::make_pair(i, this->size - 1), dLine2, player, this->size - i);
	}

	return score;
}

float hPlayerMultiplier = 1;

int Gomoku::heuristic() {

	int score0 = this->heuristicByPlayer(0);
	int score1 = this->heuristicByPlayer(1);
	return this->heuristicPlayer->index == 0 ? hPlayerMultiplier * score0 - score1 :  hPlayerMultiplier * score1 - score0;

	// return this->heuristicPlayer->index == 0 ? score0 : score1;
	// return finalScore;
}


std::vector<AAction*> Gomoku::doMove(std::pair<int, int>& pos) {
	std::vector<AAction*> actions = this->place(pos.first, pos.second, this->currentPlayer->index);
	this->switchPlayer();
	return actions;
}

bool Gomoku::canPlace(std::pair<int, int> move) const {
	for (auto rule: this->rules) {
		if (!rule->canPlace(*Gomoku::gomoku, move)) {
			return false;
		}
	}
	return true;
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
			if (board[j][i] < Minmax::INF_MIN + 20) {
				std::cout << std::setw(6) << "m" + std::to_string(std::abs(board[j][i] % 10)) << " ";
			} else if (board[j][i] > Minmax::INF_MAX - 20) {
				std::cout << std::setw(6) << "M" + std::to_string(std::abs(board[j][i] % 10))  << " ";
			} else {
				std::cout << std::setw(6) << board[j][i] << " ";
			}
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
	auto pos = Gomoku::gomoku->minmax->run();

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
	Gomoku::gomoku->stackActions.push_back(Gomoku::gomoku->place(y, x, Gomoku::gomoku->currentPlayer->index));
	return PyLong_FromLong(0);
}

PyObject* Gomoku::undo(PyObject* self, PyObject* args) {
	if (Gomoku::gomoku->stackActions.size()) {
		auto actions = Gomoku::gomoku->stackActions.back();
		Gomoku::gomoku->stackActions.pop_back();
		Gomoku::gomoku->undoMove(actions);
	}
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
	
	if (!PyArg_ParseTuple(args, "b", &Gomoku::gomoku->playing)) {
		return NULL;
	}

	return PyLong_FromLong(0);
}

PyObject* Gomoku::isCurrentPlayerAI(PyObject* self, PyObject* args) {
	return PyBool_FromLong(Gomoku::gomoku->currentPlayer->type == Player::AI);
}

PyObject* Gomoku::canPlace(PyObject* self, PyObject* args) {
	int y, x;
	
	if (!PyArg_ParseTuple(args, "ii", &y, &x)) {
		return NULL;
	}

	return PyBool_FromLong(Gomoku::gomoku->canPlace(std::make_pair(y, x)));
}


PyObject* Gomoku::setPlayerType(PyObject* self, PyObject* args) {
	int playerIndex;
	int playerType;
	

	if (!PyArg_ParseTuple(args, "ii", &playerIndex, &playerType)) {
		return NULL;
	}

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
	{"can_place", Gomoku::canPlace, METH_VARARGS, "fuck the eval_line_test pouet machine."},
	// {"undo", Gomoku::undo, METH_VARARGS, "Returns the undo pouet machines."},
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
	// std::cout << "==============";
	for (int i = 0; i < size; i ++) {
		if (i == size - 1) {
			std::cout << line[i];
		} else {
			std::cout << line[i] << ", ";
		}
	}
	std::cout << "\t => score: " << score << std::endl;
}

PyObject* Gomoku::testEvalLine(PyObject* self, PyObject* args) {
	std::cout << "testEvalLine" << std::endl;
	Gomoku::gomoku = new Gomoku(10, Player::Type::AI, Player::Type::AI);

	Gomoku::gomoku->board = std::vector<std::vector<int>>();

	std::pair<int, int> dir = std::make_pair(0, 1);

	int player = 0;
	Gomoku::gomoku->heuristicPlayer = &Gomoku::gomoku->players[0];
	
	// Streak length tests
	std::cout << "Streak length tests" << std::endl;
	{
		int lines[][10] = {
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, 0, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, 0, 0, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, 0, 0, 0, -1, -1, -1, -1, -1, -1 },
			{ -1, 0, 0, 0, 0, -1, -1, -1, -1, -1 },
			{ -1, 0, 0, 0, 0, 0, -1, -1, -1, -1 },
			{ -1, 0, 0, 0, 0, 0, 0, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		};

		for (auto &line: lines) {
			Gomoku::gomoku->board.push_back(std::vector<int> (std::begin(line), std::end(line)));
		} 
		for (int i = 0; i < 7; i++) {
			int ret = Gomoku::gomoku->evalLine(std::make_pair(i, 0), dir, player, 10);
			printLineScore(Gomoku::gomoku->board[i], 10, ret);
		}
	}

	// Half opened tests
	std::cout << "Half opened tests" << std::endl;
	Gomoku::gomoku->board.clear();
	{
		int lines[][10] = {
			{-1, 0, 0, -1, -1, -1, -1, -1, -1, -1 },
			{ 1, 0, 0, -1, -1, -1, -1, -1, -1, -1 },
			{ 0, 0, -1, -1, -1, -1, -1, -1, -1, -1 },

			{-1, 0, 0, 0, -1, -1, -1, -1, -1, -1 },
			{ 1, 0, 0, 0, -1, -1, -1, -1, -1, -1 },
			{ 0, 0, 0, -1, -1, -1, -1, -1, -1, -1 },

			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		};

		for (auto &line: lines) {
			Gomoku::gomoku->board.push_back(std::vector<int> (std::begin(line), std::end(line)));
		} 
		for (int i = 0; i < 6; i++) {
			int ret = Gomoku::gomoku->evalLine(std::make_pair(i, 0), dir, player, 10);
			printLineScore(Gomoku::gomoku->board[i], 10, ret);
		}
	}
	// Discontinued tests
	std::cout << "Discontinued tests" << std::endl;
	Gomoku::gomoku->board.clear();
	{
		int lines[][10] = {
			{ -1, 0, 0, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, 0, -1, 0, -1, -1, -1, -1, -1, -1 },

			{ -1, 0, 0, 0, -1, -1, -1, -1, -1, -1 },
			{ -1, 0, 0, -1, 0, -1, -1, -1, -1, -1 },
			
			{ -1, 0, 0, 0, 0, -1, -1, -1, -1, -1 },
			{ -1, 0, 0, -1, 0, 0, -1, -1, -1, -1 },
			{ -1, 0, -1, 0, 0, 0, -1, -1, -1, -1 },
			
			{ -1, 0, 0, 0, 0, 0, -1, -1, -1, -1 },
			{ -1, 0, 0, -1, 0, 0, 0, -1, -1, -1 },
			{ -1, 0, -1, -1, 0, 0, 0, 0, -1, -1 },
		};

		for (auto &line: lines) {
			Gomoku::gomoku->board.push_back(std::vector<int> (std::begin(line), std::end(line)));
		} 
		for (int i = 0; i < 10; i++) {
			int ret = Gomoku::gomoku->evalLine(std::make_pair(i, 0), dir, player, 10);
			printLineScore(Gomoku::gomoku->board[i], 10, ret);
		}
	}
	return PyLong_FromLong(0);
}
