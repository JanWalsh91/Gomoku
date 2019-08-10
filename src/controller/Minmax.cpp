#include "Minmax.hpp"

#define PRUNNING true
#define PROXIMITY_BONUS false
#define DEBUG_POS false
#define RANDOM_POS true

Minmax::Minmax(Gomoku& gomoku, int maxDepth): maxDepth(maxDepth), gomoku(gomoku), _running(false) {}

bool Minmax::isRunning() const {
	return this->_running;
}

// this->minmaxAlphaBeta(depth, −∞, +∞, TRUE)
std::pair<int, int> Minmax::run() {
	this->_running = true;

	this->gomoku.heuristicPlayer = this->gomoku.currentPlayer;
	std::cout << "Minmax run " << maxDepth << " Heuritic player: " << this->gomoku.heuristicPlayer->getIndex() << std::endl;
	
	this->bestMove = std::make_pair(-1, -1);
	this->bestValue = Minmax::INF_MIN;
	
	this->heuristicValues = std::vector<std::vector<int>>(this->gomoku.size, std::vector<int>(this->gomoku.size, 0));
	
	this->minmaxAlphaBeta(this->maxDepth, Minmax::INF_MIN, Minmax::INF_MAX, true, true, 0);

	// this->gomoku.printBoard(this->gomoku.board, this->bestMove);
	std::cout << "heuristicValues for player " << this->gomoku.currentPlayer->getIndex() << std::endl;
	this->gomoku.printBoard(this->heuristicValues, this->bestMove);
	this->gomoku.printBoard(this->gomoku.board, this->bestMove);
	std::cout << std::endl;

	this->_running = false;

	return this->bestMove;
}

#if DEBUG_POS
auto _boards = std::vector<std::vector<std::vector<int>>>() = {
	std::vector<std::vector<int>>(13, std::vector<int>(13, 0)),
	std::vector<std::vector<int>>(13, std::vector<int>(13, 0)),
	std::vector<std::vector<int>>(13, std::vector<int>(13, 0)),
	std::vector<std::vector<int>>(13, std::vector<int>(13, 0)),
	std::vector<std::vector<int>>(13, std::vector<int>(13, 0))
};

auto _bestMove = std::vector<std::pair<int, int>>() =  {
	std::make_pair(-1, -1),
	std::make_pair(-1, -1),
	std::make_pair(-1, -1),
	std::make_pair(-1, -1),
	std::make_pair(-1, -1)
};

auto _bestValue = std::vector<int>() =  {
	0,
	0,
	0,
	0,
	0
};

// -1: don't display
// -2: always display
// pos pos player
auto displayAt = std::vector<std::tuple<int, int, int>>() =  {
	std::make_tuple(1, 1, 0),
	std::make_tuple(6, 4, 1),
	std::make_tuple(1, 2, -2),
	std::make_tuple(1, 2, -2),
	std::make_tuple(1, 2, -2)



// /* DEPTH */
		
// /*  0  */	std::make_tuple(-1, -1, 1),

// /*  1  */	std::make_tuple(-1, -1, 0),
			
// /*  2  */	std::make_tuple(-1, -1, 1),

// // /*  3  */	std::make_tuple(6, 4, 0),
// /*  3  */	std::make_tuple(1, 2, 0),

// // /*  4  */	std::make_tuple(-1, -1, 1),
	
};

bool shouldDisplay(Gomoku& gomoku, int depth) {
	// std::cout << "Should display" << std::endl;
	int d = gomoku.minmax->maxDepth - depth - 1;

	// std::cout << "d: " << d << std::endl;

	for (int i = 0; i <= d; i++) {
		if (gomoku.getValueOnBoard(std::make_pair(std::get<0>(displayAt[i]), std::get<1>(displayAt[i]))) != std::get<2>(displayAt[i])) {
			return false;
		}
	}
	return true;
	// for (int i = gomoku.minmax->maxDepth; i > depth; i--) {
	// 	if (std::get<0>(displayAt[i]) == -1 || (std::get<0>(displayAt[i]) != -2 && gomoku.board[std::get<0>(displayAt[i])][std::get<1>(displayAt[i])] != std::get<2>(displayAt[i]))) {
	// 		return false;
	// 	}
	// }
	// return true;
}

void displayDebug(Gomoku& gomoku, bool maximizing, int alpha, int beta, int depth) {
	std::cout << "\n======================== DEBUG =================" << std::endl;
	std::cout << "MAXIMIZING for player " << gomoku.currentPlayer->getIndex() << ". a: " << alpha << ", b: " << beta << " at depth " << depth << ", max ? " << maximizing << std::endl;
	std::cout << "Board: " << std::endl;
	gomoku.printBoard();
	std::cout << "Values: " << std::endl;
	gomoku.printBoard(_boards[depth], _bestMove[depth]);
	std::cout << "===============================================\n" << std::endl;
}
#endif

bool showed[] = {false, false, false, false, false};

int Minmax::minmaxAlphaBeta(int depth, int alpha, int beta, bool maximizing, bool root, int heuristicValue) {
	// std::cout << "minmaxAlphaBeta depth: " << depth << ", endState: " << this->gomoku.endState    				<< ", H: " << heuristicValue << std::endl;
	// std::cout << "m" << (maximizing ? "ax" : "in") << "imizing" << std::endl;
	#if DEBUG_POS
	_boards[depth] = std::vector<std::vector<int>>(this->gomoku.size, std::vector<int>(this->gomoku.size, 0));
	_bestMove[depth] = std::make_pair(-1, -1);	
	#endif

	if (this->gomoku.endState != Gomoku::PLAYING) {
		if (this->gomoku.endState >= 0) {
			if (maximizing) {
				return Minmax::INF_MIN + (this->maxDepth - depth);
			} else {
				return Minmax::INF_MAX - (this->maxDepth - depth);
			}
		} else {
			return 0;
		}
	}

	if (depth == 0) {
		return heuristicValue;
	}

	auto moves = this->gomoku.getMoves();
	auto heuristicsByMove = this->getSortedMoves(moves, maximizing, depth);
	// std::cout << "HERE just got sorted moves  at depth " << depth << std::endl;

	// if (!showed[3] && depth == 3) {
	// 	std::cout << "Number of moves: " << heuristicsByMove.size() << std::endl;
	// 	std::cout << "h player: " << this->gomoku.heuristicPlayer->getIndex() << std::endl;
	// 	this->gomoku.printBoard();
	// 	for (int i = 0; i < heuristicsByMove.size(); i++) {
	// 		std::cout << heuristicsByMove[i].move << ":  " << heuristicsByMove[i].heuristic << std::endl;
	// 	}
	// 	showed[3] = true;
	// 	std::cout << "===========" << std::endl;
	// }

	// if (!showed[2] && depth == 2) {
	// 	std::cout << "Number of moves: " << heuristicsByMove.size() << std::endl;
	// 	for (int i = 0; i < heuristicsByMove.size(); i++) {
	// 		std::cout << heuristicsByMove[i].move << ":  " << heuristicsByMove[i].heuristic << std::endl;
	// 	}
	// 	showed[2] = true;
	// }




	if (maximizing) {
		int value = Minmax::INF_MIN - 1;
		
		#if DEBUG_POS
		_bestValue[depth] = value;
		#endif

		// Return early if Heuristic found end state (can't find anything better!)
		// if (heuristicsByMove[0].heuristic <= -20'000 + 10) {
		// 	// std::cout << "Heuristic found end state (0): " << heuristicsByMove[0].heuristic << std::endl;
		// 	if (root) {
		// 		this->bestValue = heuristicsByMove[0].heuristic;
		// 		this->bestMove = heuristicsByMove[0].move;
		// 		this->heuristicValues[heuristicsByMove[0].move.first][heuristicsByMove[0].move.second] = heuristicsByMove[0].heuristic;
		// 	}
		// 	#if DEBUG_POS
		// 	_bestMove[depth] = heuristicsByMove[0].move;
		// 	_boards[depth][heuristicsByMove[0].move.first][heuristicsByMove[0].move.second] = heuristicsByMove[0].heuristic;
		// 	if (shouldDisplay(this->gomoku, depth)) displayDebug(this->gomoku, maximizing, alpha, beta, depth);
		// 	#endif
		
		// 	return heuristicsByMove[0].heuristic;
		// }
		// int count = 0;

		for (auto& heuristicByMove: heuristicsByMove) {
			auto undoMoves = this->gomoku.doMove(heuristicByMove.move);
			int ret = this->minmaxAlphaBeta(depth - 1, alpha, beta, false, false, heuristicByMove.heuristic);
			this->gomoku.undoMove(undoMoves);
			
			#if DEBUG_POS
			if (ret > _bestValue[depth]) {
				_bestValue[depth] = ret;
				_bestMove[depth] = heuristicByMove.move;
			}
			#endif

			value = std::max(ret, value);
			alpha = std::max(alpha, value);

			#if DEBUG_POS
			_boards[depth][heuristicByMove.move.first][heuristicByMove.move.second] = ret;
			#endif

			if (root) {
				#if PROXIMITY_BONUS
				for (auto lastMove : this->gomoku.lastMoves) {
					if (lastMove.first != -1) {
						if (std::abs(heuristicByMove.move.first - lastMove.first) == 1 && std::abs(heuristicByMove.move.second - lastMove.second) == 1) {
							ret += Minmax::PROXIMITY_BONUNS;
							break;
						}
					}
				}
				#endif

				#if RANDOM_POS
				if (ret == this->bestValue && std::rand() % 4 == 0) {
					this->bestValue = ret;
					this->bestMove = heuristicByMove.move;
				}
				#endif
				
				if (ret > this->bestValue) {
					this->bestValue = ret;
					this->bestMove = heuristicByMove.move;
					
					#if DEBUG_POS
					_bestValue[depth] = ret;
					_bestMove[depth] = heuristicByMove.move;
					#endif
				}
				this->heuristicValues[heuristicByMove.move.first][heuristicByMove.move.second] = ret;
			}

			if (alpha >= beta) {
				#if PRUNNING
				break;
				#endif
			}
		}

		#if DEBUG_POS
		if (shouldDisplay(this->gomoku, depth)) displayDebug(this->gomoku, maximizing, alpha, beta, depth);
		#endif
			
		return value;

	} else {
		int value = Minmax::INF_MAX + 1;

		#if DEBUG_POS
		_bestValue[depth] = value;
		#endif

		// Return early if Heuristic found end state (can't find anything worse!)
		// if (heuristicsByMove[0].heuristic >= 20'000 - 10) {
		// 	// std::cout << "Heuristic found end state (1): " << heuristicsByMove[0].heuristic << std::endl;
		// 	#if DEBUG_POS
		// 	_bestMove[depth] = heuristicsByMove[0].move;
		// 	_boards[depth][heuristicsByMove[0].move.first][heuristicsByMove[0].move.second] = heuristicsByMove[0].heuristic;
		// 	#endif
		// 	#if DEBUG_POS
		// 	_bestMove[depth] = heuristicsByMove[0].move;
		// 	_boards[depth][heuristicsByMove[0].move.first][heuristicsByMove[0].move.second] = heuristicsByMove[0].heuristic;
		// 	if (shouldDisplay(this->gomoku, depth)) displayDebug(this->gomoku, maximizing, alpha, beta, depth);
		// 	#endif
		// 	return heuristicsByMove[heuristicsByMove.size() - 1].heuristic;
		// }

		for (auto& heuristicByMove: heuristicsByMove) {
			auto undoMoves = this->gomoku.doMove(heuristicByMove.move);
			int ret = this->minmaxAlphaBeta(depth - 1, alpha, beta, true, false, heuristicByMove.heuristic);
			this->gomoku.undoMove(undoMoves);

			#if DEBUG_POS
			if (ret < _bestValue[depth]) {
				_bestValue[depth] = ret;
				_bestMove[depth] = heuristicByMove.move;
			}
			#endif

			value = std::min(ret, value);
			beta = std::min(beta, value);

			#if DEBUG_POS
			_boards[depth][heuristicByMove.move.first][heuristicByMove.move.second] = ret;
			#endif

			if (alpha >= beta) {
				#if PRUNNING
				break;
				#endif
			}
		}

		#if DEBUG_POS
		if (shouldDisplay(this->gomoku, depth)) displayDebug(this->gomoku, maximizing, alpha, beta, depth);
		#endif

		return value;
	}
	// if (depth == 3) {
	// 	exit(0);
	// }

}

std::vector<Minmax::HeuristicByMove> Minmax::getSortedMoves(std::vector<std::pair<int, int>>& moves, bool maximizing, int depth) const {
	std::vector<Minmax::HeuristicByMove> heuristicsByMove;

	std::shared_ptr<Player> currentPlayer = this->gomoku.currentPlayer;
	std::shared_ptr<Player> otherPlayer = this->gomoku.players[this->gomoku.currentPlayer->getIndex() == 0 ? 1 : 0];

	for (auto& move: moves) {
		auto undoMoves = this->gomoku.doMove(move);

		if (this->gomoku.endState >= 0) {
			// if (depth == 3) {
			// 	std::cout << "END STATE FOUND for move " << move << std::endl;
			// }
			if (maximizing) {
				heuristicsByMove.push_back( {
					move,
					Minmax::INF_MAX - (this->maxDepth - depth),
				});
			} else {
				heuristicsByMove.push_back({
					move,
					Minmax::INF_MIN + (this->maxDepth - depth),
				});
			}
		} else if (this->gomoku.endState == Gomoku::State::DRAW) {
			heuristicsByMove.push_back({move, 0});
		} else {
			Minmax::HeuristicByMove hbm;

			hbm.move = move;
			hbm.heuristic = this->gomoku.heuristic(depth);
			
			heuristicsByMove.push_back(hbm);
		}
		
		this->gomoku.undoMove(undoMoves);
	}

	std::vector<Minmax::HeuristicByMove> sortedHeuristicByMove;
	if (maximizing) {
		while (heuristicsByMove.size()/* && sortedMoves.size() < 10*/) {
			int maxValue = std::numeric_limits<int>::min();
			int maxValueIndex = -1;
			for (unsigned i = 0; i < moves.size(); i++) {
				if (heuristicsByMove[i].heuristic > maxValue) {
					maxValueIndex = i;
					maxValue = heuristicsByMove[i].heuristic;
				}
			}
			sortedHeuristicByMove.push_back(heuristicsByMove[maxValueIndex]);
			heuristicsByMove.erase(heuristicsByMove.begin() + maxValueIndex);
			moves.erase(moves.begin() + maxValueIndex);
		}
	}
	else {
		while (heuristicsByMove.size()/* && sortedMoves.size() < 10*/) {
			int minValue = std::numeric_limits<int>::max();
			int minValueIndex = -1;
			for (unsigned i = 0; i < moves.size(); i++) {
				if (heuristicsByMove[i].heuristic < minValue) {
					minValueIndex = i;
					minValue = heuristicsByMove[i].heuristic;
				}
			}
			sortedHeuristicByMove.push_back(heuristicsByMove[minValueIndex]);
			heuristicsByMove.erase(heuristicsByMove.begin() + minValueIndex);
			moves.erase(moves.begin() + minValueIndex);
		}
	}
	return sortedHeuristicByMove;
}
