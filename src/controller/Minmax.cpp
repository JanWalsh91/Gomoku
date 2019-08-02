#include "Minmax.hpp"

Minmax::Minmax(Gomoku& gomoku, int maxDepth): maxDepth(maxDepth), gomoku(gomoku) {}
std::ostream & operator << (std::ostream &out, std::pair<int, int> &c) {
    out << "[" << c.first << ", " << c.second << "]";
    return out;
}

// this->minmaxAlphaBeta(depth, −∞, +∞, TRUE)
std::pair<int, int> Minmax::run() {
	this->gomoku.heuristicPlayer = this->gomoku.currentPlayer;
	std::cout << "Minmax run " << maxDepth << " Heuritic player: " << this->gomoku.heuristicPlayer->index << std::endl;
	
	this->bestMove = std::make_pair(-1, -1);
	this->bestValue = Minmax::INF_MIN;
	
	this->heuristicValues = std::vector<std::vector<int>>(this->gomoku.size, std::vector<int>(this->gomoku.size, 0));
	
	this->minmaxAlphaBeta(this->maxDepth, Minmax::INF_MIN, Minmax::INF_MAX, true, true, 0);

	// auto moves = this->gomoku.getMoves();
	
	// // int alpha = Minmax::INF_MIN;
	// // int beta = Minmax::INF_MAX;

	// for (auto &move: moves) {
	// 	auto undoMoves = this->gomoku.doMove(move);

	// 														// TODO: hum?
	// 	int ret = this->minmaxAlphaBeta(this->maxDepth - 1, Minmax::INF_MIN, Minmax::INF_MAX, false);
	// 	// this->gomoku.printBoard(this->gomoku.board, move);
	// 	// std::cout << "minmaxAlphaBeta ret: " << ret << std::endl;
	// 	// getchar();
	// 	if (ret > this->bestValue) {
	// 		// std::cout << "ret(" << ret << ") > bestValue(" << this->bestValue << ")" << std::endl; 
	// 		this->bestMove = move;
	// 		this->bestValue = ret;
	// 		// TODO ret == alpha or beta?
	// 	}
	// 	this->heuristicValues[move.first][move.second] = ret;

	// 	this->gomoku.undoMove(undoMoves);
	// }


	this->gomoku.printBoard(this->gomoku.board, this->bestMove);
	std::cout << "heuristicValues for player " << this->gomoku.currentPlayer->index << std::endl;
	this->gomoku.printBoard(this->heuristicValues, this->bestMove);
	std::cout << std::endl;

	return this->bestMove;
}

// auto _boards = std::vector<std::vector<std::vector<int>>>() = {
// 	std::vector<std::vector<int>>(9, std::vector<int>(9, 0)),
// 	std::vector<std::vector<int>>(9, std::vector<int>(9, 0)),
// 	std::vector<std::vector<int>>(9, std::vector<int>(9, 0)),
// 	std::vector<std::vector<int>>(9, std::vector<int>(9, 0)),
// 	std::vector<std::vector<int>>(9, std::vector<int>(9, 0))
// };

// auto _bestMove = std::vector<std::pair<int, int>>() =  {
// 	std::make_pair(-1, -1),
// 	std::make_pair(-1, -1),
// 	std::make_pair(-1, -1),
// 	std::make_pair(-1, -1),
// 	std::make_pair(-1, -1)
// };

// auto _bestValue = std::vector<int>() =  {
// 	0,
// 	0,
// 	0,
// 	0,
// 	0
// };

// -1: don't display
// -2: always display
// pos pos player
auto displayAt = std::vector<std::tuple<int, int, int>>() =  {
		std::make_tuple(-1, -1, 0),

		std::make_tuple(-1, -1, 1),
		
		std::make_tuple(-1, -1, 0),
		// std::make_tuple(5, 3, 0),
		// std::make_tuple(7, 3, 0),
		
		// std::make_tuple(2, 6, 1),
		std::make_tuple(-1, -1, 1),

		std::make_tuple(-1, -1, 0),
	
	// std::make_pair(-2, -1)			// 3
};

bool shouldDisplay(Gomoku& gomoku, int depth) {
	// std::cout << "shouldDisplay depth: " << depth << "maxDepth: " << gomoku.minmax->maxDepth << std::endl;  
	for (int i = gomoku.minmax->maxDepth; i > depth; i--) {
		// std::cout << "i: " << i << std::endl;
		if (std::get<0>(displayAt[i]) == -1 || (std::get<0>(displayAt[i]) != -2 && gomoku.board[std::get<0>(displayAt[i])][std::get<1>(displayAt[i])] != std::get<2>(displayAt[i]))) {
			// std::cout << "false" << std::endl;
			return false;
		}
	}
	// std::cout << "true" << std::endl;
	return true;
}

auto boardD2 = std::vector<std::vector<int>>(9, std::vector<int>(9, 0));
auto boardD1 = std::vector<std::vector<int>>(9, std::vector<int>(9, 0));

// std::vector<std::vector<int>> boardD2;
bool isinit2 = false;

int Minmax::minmaxAlphaBeta(int depth, int alpha, int beta, bool maximizing, bool root, int heuristicValue) {
	// getchar();

	// reset board, bestMove
	// _boards[depth] = std::vector<std::vector<int>>(this->gomoku.size, std::vector<int>(this->gomoku.size, 0));
	// _bestMove[depth] = std::make_pair(-1, -1);

	if (this->gomoku.endState != Gomoku::PLAYING) {
		// Player 0 or 1 :)
		if (this->gomoku.endState >= 0) {
			if (maximizing) {
				return Minmax::INF_MIN + (this->maxDepth - depth);
			} else {
				return Minmax::INF_MAX - (this->maxDepth - depth);
			}
		} else {
			// Draw
			return 0;
		}
	}

	if (depth == 0) {
		// int h = this->gomoku.heuristic();
		// return h;
		return heuristicValue; // with love 🇨🇦ᔉ
	}

	auto moves = this->gomoku.getMoves();
	auto sortedMoves = this->getSortedMoves(moves, maximizing, depth);

	// if (depth == 2) {
		// this->gomoku.printBoard();
		// std::cout << "Moves:" << std::endl;
		// for (auto& move: moves) {
		// 	std::cout << move << std::endl;
		// }
		// std::cout << "Movesded" << std::endl;
	// }

	if (maximizing) {
		int value = Minmax::INF_MIN;
		// _bestValue[depth] = value;

		for (auto& move: sortedMoves) {
			auto undoMoves = this->gomoku.doMove(move.second);
			int ret = this->minmaxAlphaBeta(depth - 1, alpha, beta, false, false, move.first); // TODO: pass heuristic results if depth will == 0
			this->gomoku.undoMove(undoMoves);
			
			// if (ret > _bestValue[depth]) {
			// 	_bestValue[depth] = ret;
			// 	_bestMove[depth] = move;
			// }

			value = std::max(ret, value);
			alpha = std::max(alpha, value);

			// _boards[depth][move.first][move.second] = ret;

			if (alpha >= beta) {
				break;
			}


			if (root) {
				for (auto lastMove : this->gomoku.lastMoves) {
					if (lastMove.first != -1) {
						if (std::abs(move.second.first - lastMove.first) == 1 && std::abs(move.second.second - lastMove.second) == 1) {
							ret += Minmax::PROXIMITY_BONUNS;
							break;
						}
					}
				}

				if (ret > this->bestValue) {
					this->bestValue = ret;
					this->bestMove = move.second;
				}
				this->heuristicValues[move.second.first][move.second.second] = ret;
			}
			

		}

		// if (shouldDisplay(this->gomoku, depth)) {
		// 	std::cout << "MAXIMIZING for player " << this->gomoku.currentPlayer->index << ". a: " << alpha << ", b: " << beta << " at depth " << depth << std::endl;
		// 	std::cout << "Board: " << std::endl;
		// 	this->gomoku.printBoard();
		// 	std::cout << "Values: " << std::endl;
		// 	this->gomoku.printBoard(_boards[depth], _bestMove[depth]);
		// 	std::cout << "===============================================" << std::endl;
		// }
			
		return value;

	} else {
		int value = Minmax::INF_MAX;
		// _bestValue[depth] = value;

		for (auto& move: sortedMoves) {
			auto undoMoves = this->gomoku.doMove(move.second);
			int ret = this->minmaxAlphaBeta(depth - 1, alpha, beta, true, false, move.first);
			this->gomoku.undoMove(undoMoves);

			// if (ret < _bestValue[depth]) {
			// 	_bestValue[depth] = ret;
			// 	_bestMove[depth] = move;
			// }

			value = std::min(ret, value);
			beta = std::min(beta, value);

			// _boards[depth][move.first][move.second] = ret;

			if (alpha >= beta) {
				break;
			}
		}

		// if (shouldDisplay(this->gomoku, depth)) {
		// 	std::cout << "MINIMIZING for player " << this->gomoku.currentPlayer->index << ". a: " << alpha << ", b: " << beta << " at depth " << depth << std::endl;
		// 	std::cout << "Board: " << std::endl;
		// 	this->gomoku.printBoard();
		// 	std::cout << "Values: " << std::endl;
		// 	this->gomoku.printBoard(_boards[depth], _bestMove[depth]);
		// 	std::cout << "===============================================" << std::endl;
		// }

		return value;
	}

}

std::vector<std::pair<int, std::pair<int, int>>> Minmax::getSortedMoves(std::vector<std::pair<int, int>>& moves, bool maximizing, int depth) const {
	auto sortedMoves = std::vector<std::pair<int, std::pair<int, int>>>();
	
	std::vector<int> movesHeuristicValues;
	for (auto& move: moves) {
		auto undoMoves = this->gomoku.doMove(move);

		if (this->gomoku.endState >= 0) {
			if (maximizing) {
				movesHeuristicValues.push_back(Minmax::INF_MIN + (this->maxDepth - depth));
			} else {
				movesHeuristicValues.push_back(Minmax::INF_MAX - (this->maxDepth - depth));
			}
		} else if (this->gomoku.endState == Gomoku::State::DRAW) {
			movesHeuristicValues.push_back(0);
		} else {
			movesHeuristicValues.push_back(this->gomoku.heuristic());
		}
		
		this->gomoku.undoMove(undoMoves);
	}
	if (maximizing) {
		while (movesHeuristicValues.size()/* && sortedMoves.size() < 10*/) {
			int maxValue = std::numeric_limits<int>::min();
			int maxValueIndex = -1;
			for (unsigned i = 0; i < moves.size(); i++) {
				if (movesHeuristicValues[i] > maxValue) {
					maxValueIndex = i;
					maxValue = movesHeuristicValues[i];
				}
			}
			sortedMoves.push_back(std::make_pair(maxValue, moves[maxValueIndex]));
			movesHeuristicValues.erase(movesHeuristicValues.begin() + maxValueIndex);
			moves.erase(moves.begin() + maxValueIndex);
		}
	}
	else {
		while (movesHeuristicValues.size()/* && sortedMoves.size() < 10*/) {
			int minValue = std::numeric_limits<int>::max();
			int minValueIndex = -1;
			for (unsigned i = 0; i < moves.size(); i++) {
				if (movesHeuristicValues[i] < minValue) {
					minValueIndex = i;
					minValue = movesHeuristicValues[i];
				}
			}
			sortedMoves.push_back(std::make_pair(minValue, moves[minValueIndex]));
			movesHeuristicValues.erase(movesHeuristicValues.begin() + minValueIndex);
			moves.erase(moves.begin() + minValueIndex);
		}
	}
	return sortedMoves;
}
