#include "Minmax.hpp"

Minmax::Minmax(Gomoku& gomoku, int maxDepth): maxDepth(maxDepth), gomoku(gomoku) {}
std::ostream & operator << (std::ostream &out, std::pair<int, int> &c) {
    out << "[" << c.first << ", " << c.second << "]";
    return out;
}

// this->minmaxAlphaBeta(depth, −∞, +∞, TRUE)
std::pair<int, int> Minmax::run() {
	std::cout << "Minmax run " << maxDepth << std::endl;
	this->gomoku.heuristicPlayer = this->gomoku.currentPlayer;
	
	this->bestMove = std::make_pair(-1, -1);
	this->bestValue = Minmax::INF_MIN;
	
	this->heuristicValues = std::vector<std::vector<int>>(this->gomoku.size, std::vector<int>(this->gomoku.size, 0));
	
	this->minmaxAlphaBeta(this->maxDepth, Minmax::INF_MIN, Minmax::INF_MAX, true, true);


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


	std::cout << "heuristicValues for player " << this->gomoku.currentPlayer->index << std::endl;
	this->gomoku.printBoard(this->heuristicValues, this->bestMove);
	std::cout << std::endl;

	return this->bestMove;
}


std::vector<std::vector<int>> boardD2;
bool isinit2 = false;

int Minmax::minmaxAlphaBeta(int depth, int alpha, int beta, bool maximizing, bool root) {
	// std::cout << "⬇️  minmaxAlphaBeta depth " << depth << ", a: " << alpha << ", b: " << beta << std::endl; 
	// getchar();
	bool record2 = false;


	if (depth == 2 && this->gomoku.board[4][2] == 0) {
		record2 = true;
		if (!isinit2) {
			boardD2 = std::vector<std::vector<int>>(this->gomoku.size, std::vector<int>(this->gomoku.size, 0));
			isinit2 = true;
		}
	}

	if (this->gomoku.endState != Gomoku::PLAYING) {
		// Player 0 or 1 :)
		if (this->gomoku.endState >= 0) {
			if (maximizing) {
				// std::cout << "⬆️  End State for Maximizing. Return + INF" << std::endl; 
				return Minmax::INF_MIN + (this->maxDepth - depth);
			} else {
				// std::cout << "⬆️  End State for Minimizing. Return - INF" << std::endl; 
				return Minmax::INF_MAX - (this->maxDepth - depth);
			}
		} else {
			// std::cout << "⬆️  End State Draw. Return - 0" << std::endl; 
			// Draw
			return 0;
		}
	}

	if (depth == 0) {
		int h = this->gomoku.heuristic();
		// std::cout << "⬆️  Leaf node. Return " << h << std::endl; 
		return h;
	}

	auto moves = this->gomoku.getMoves();
	// moves = this->getSortedMoves(moves, maximizing, depth);

	if (maximizing) {

		int value = Minmax::INF_MIN;

		for (auto& move: moves) {
			auto undoMoves = this->gomoku.doMove(move);
			
			int ret = this->minmaxAlphaBeta(depth - 1, alpha, beta, false, false); // TODO: pass heuristic results if depth will == 0
			this->gomoku.undoMove(undoMoves);
			
			value = std::max(ret, value);
			alpha = std::max(alpha, value);

			if (alpha >= beta) {
				break;
			}

			if (root) {
				// this->gomoku.printBoard();
				if (ret > this->bestValue) {
					this->bestValue = ret;
					this->bestMove = move;
				}
				this->heuristicValues[move.first][move.second] = ret;
			}
			

		}
		return value;

	} else {

		int value = Minmax::INF_MAX;

		for (auto& move: moves) {
			auto undoMoves = this->gomoku.doMove(move);
			int ret = this->minmaxAlphaBeta(depth - 1, alpha, beta, true, false);
			this->gomoku.undoMove(undoMoves);	

			value = std::min(ret, value);
			beta = std::min(beta, value);

			if (alpha >= beta) {
				break;
			}
		}

		return value;
	}

}

std::vector<std::pair<int, int>> Minmax::getSortedMoves(std::vector<std::pair<int, int>>& moves, bool maximizing, int depth) const {
	auto sortedMoves = std::vector<std::pair<int, int>>();
	
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
			sortedMoves.push_back(moves[maxValueIndex]);
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
			sortedMoves.push_back(moves[minValueIndex]);
			movesHeuristicValues.erase(movesHeuristicValues.begin() + minValueIndex);
			moves.erase(moves.begin() + minValueIndex);
		}
	}
	return sortedMoves;
}
