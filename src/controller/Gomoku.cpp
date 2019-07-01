#include <Python.h>
#include <iostream>
#include <sstream>

#include <iomanip>

#include "Gomoku.hpp"

// TODO: is PLaying, reset function
Gomoku::Gomoku(int size): size(size), isPlaying(true) {
	this->players.push_back(Player(0, Player::HUMAN));
	this->players.push_back(Player(1, Player::AI));

	this->board = std::vector<std::vector<int>>(size, std::vector<int>(size, -1));

	this->currentPlayer = &this->players[0];
	this->heuristicPlayer = nullptr;
	this->lastMoves = std::vector<std::pair<int, int>>(2, std::make_pair<int, int>(-1, -1));

	this->hashState();
}

void Gomoku::place(int& y, int& x, int& playerIndex) {
	std::cout << "Placing " << y << ", " << x << ", " << playerIndex << std::endl;
	// std::cout << this->board.size() << std::endl;
	this->board[y][x] = playerIndex;
	this->lastMoves[playerIndex] = std::make_pair(y, x);
	// std::cout << "Placed " << y << ", " << x << ", " << playerIndex << std::endl;
	// this->printBoard();
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
			} else {
				empty = false;
			}
		}
	}

	if (empty) {
		moves.push_back(std::make_pair<int, int>((int)(this->size / 2), (int)(this->size / 2)));
	}

	// std::cout << "===== GET CHILD " << std::endl;
	// for (unsigned long i = 0; i < moves.size(); i++) {
		// std::cout << moves[i].first << ", " << moves[i].second << std::endl;
	// }
	// std::cout << "===== GET CHILD " << std::endl;

	return moves;
}

int Gomoku::evalLine(std::pair<int, int> start, std::pair<int, int>& line, int& player) {

	int score = 0;
	int currentStreakScore = 0;
	bool streaking = false;

	std::pair<int, int> pos;
	for (int i = 0; i < this->size; i++) {
		pos = std::make_pair<int, int>(start.first + line.first * i, start.second + line.second * i);
		if (this->board[pos.first][pos.second] == player) {
			currentStreakScore++;
			// std::cout << "increasing currentStreakScore to " << currentStreakScore << " to player " << player << " at " << "(" << pos.first << "," << pos.second << ")" << std::endl; 			
			streaking = true;
		} else {
			score += currentStreakScore * currentStreakScore;
			// std::cout << "adding " << currentStreakScore << " to player linescore " << player << " at " << "(" << pos.first << "," << pos.second << ")" << " => " << score << std::endl; 
			currentStreakScore = 0;
			streaking = false;
		}
	}

	if (streaking) {
		score += currentStreakScore * currentStreakScore;
		// std::cout << "adding " << currentStreakScore << " to player linescore " << player << " at " << "(END)" << " => " << score << std::endl; 
	}

	return score;
}


int Gomoku::heuristicByPlayer(int player) {
	int score = 0;
	std::pair<int, int> hLine = std::make_pair<int, int>(0, 1);
	std::pair<int, int> vLine = std::make_pair<int, int>(1, 0);

	for (int i = 0; i < this->size; i++) {
		int x = this->evalLine(std::make_pair(i, 0), hLine, player);
		// std::cout << "adding " << x << " to player score " << player << " at line (" << i << ", 0)" << " => " << score << std::endl; 
		score += x;
		// std::cout << "score => " << score << std::endl;
		x = this->evalLine(std::make_pair(0, i), vLine, player);
		// std::cout << "adding " << x << " to player score " << player << " at line (0, " << i << ")" << " => " << score << std::endl; 
		score += x;
		// std::cout << "score => " << score << std::endl;
	}
	return score;
}

int Gomoku::heuristic() {

	std::cout << "HEU" << std::endl;
	this->printBoard();
	int score0 = this->heuristicByPlayer(0); 
	int score1 = this->heuristicByPlayer(1);
	std::cout << std::endl << "Hooman Score: " << score0 << ", AiScore: " << score1 << std::endl;
	std::cout << std::endl;
	return score1 - (int)(score0 * 2);
	return this->heuristicByPlayer(this->heuristicPlayer->index) - this->heuristicByPlayer(this->heuristicPlayer->index == 1 ? 0 : 1);
}


void Gomoku::doMove(std::pair<int, int>& pos) {
	this->place(pos.first, pos.second, this->currentPlayer->index);
	this->switchPlayer();
}

void Gomoku::undoMove(std::pair<int, int>& pos) {
	this->board[pos.first][pos.second] = -1;
	this->switchPlayer();
}

void Gomoku::printBoard() {
	for (int j = 0; j < this->size; j++) {
		for (int i = 0; i < this->size; i++) {
			std::cout << std::setw(3) << this->board[j][i] << " ";
		}
		std::cout << std::endl;
	}
}

void Gomoku::printBoard(std::vector<std::vector<int>> board) {
	for (int j = 0; j < this->size; j++) {
		for (int i = 0; i < this->size; i++) {
			std::cout << std::setw(3) << board[j][i] << " ";
		}
		std::cout << std::endl;
	}
}

Gomoku* Gomoku::gomoku = nullptr;

PyObject* Gomoku::init(PyObject* self, PyObject* args) {
	int size;
	int maxDepth;

	if (!PyArg_ParseTuple(args, "ii", &size, &maxDepth)) {
		return NULL;
	}

	Gomoku::gomoku = new Gomoku(size);
	Gomoku::gomoku->minmax = new Minmax(*Gomoku::gomoku, maxDepth);

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
	int y, x, playerIndex;
	
	if (!PyArg_ParseTuple(args, "iii", &y, &x, &playerIndex)) {
		return NULL;
	}

	Gomoku::gomoku->place(y, x, playerIndex);
	return PyLong_FromLong(0);
}

PyObject* Gomoku::switchPlayer(PyObject* self, PyObject* args) {
	Gomoku::gomoku->switchPlayer();
	return PyLong_FromLong(0);
}



static PyMethodDef methods[] = {
	{"init", Gomoku::init, METH_VARARGS, "Returns the heuristic value."},
	{"place", Gomoku::place, METH_VARARGS, "Returns the place value."},
	{"switch_player", Gomoku::switchPlayer, METH_VARARGS, "Returns the switchPlayer value."},
	{"run", Gomoku::run, METH_VARARGS, "Returns the minmax value."},
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