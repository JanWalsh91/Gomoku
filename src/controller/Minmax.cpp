#include "Minmax.hpp"

#define PRUNNING true
#define PROXIMITY_BONUS true
#define DEBUG_POS false
#define RANDOM_POS true
#define USE_TRANSPOSITION_TABLES false

Minmax::Minmax(Gomoku& gomoku, int maxDepth): _gomoku(gomoku), _running(false), _maxDepth(maxDepth) {}

#if USE_TRANSPOSITION_TABLES
int TTCount = 0;
#endif

bool Minmax::isRunning() const {
	return _running;
}

int Minmax::getMaxDepth() const {
	return _maxDepth;
}

void Minmax::setMaxDepth(int depth) {
	_maxDepth = depth;
}

std::pair<int, int> Minmax::run() {
	_running = true;

	_gomoku.heuristicPlayer = _gomoku.currentPlayer;
	std::cout << "Minmax run for player " << _gomoku.heuristicPlayer->getIndex() << " at depth of " << _maxDepth << "." << std::endl;
	
	_bestMove = std::make_pair(-1, -1);
	_bestValue = Minmax::LOSS;
	
	_heuristicValues = std::vector<std::vector<int>>(_gomoku.size, std::vector<int>(_gomoku.size, 0));
	
	#if USE_TRANSPOSITION_TABLES
	_TT.clear();
	#endif

	Timer::Start("MinmaxRun");
	_minmaxAlphaBeta(_maxDepth, Minmax::LOSS, Minmax::VICTORY, true, true, 0);
	Timer::Stop("MinmaxRun");
	Timer::Print("MinmaxRun");

	_gomoku.printBoard(_heuristicValues, _bestMove);

	_running = false;
	return _bestMove;
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
};

bool shouldDisplay(Gomoku& gomoku, int depth) {
	int d = gomoku.minmax->maxDepth - depth - 1;
	for (int i = 0; i <= d; i++) {
		if (gomoku.getValueOnBoard(std::make_pair(std::get<0>(displayAt[i]), std::get<1>(displayAt[i]))) != std::get<2>(displayAt[i])) {
			return false;
		}
	}
	return true;
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

int Minmax::_minmaxAlphaBeta(int depth, int alpha, int beta, bool maximizing, bool root, int heuristicValue) {
	if (_gomoku.shouldReset()) {
		return -1;
	}
	#if DEBUG_POS
	_boards[depth] = std::vector<std::vector<int>>(_gomoku.size, std::vector<int>(_gomoku.size, 0));
	_bestMove[depth] = std::make_pair(-1, -1);	
	#endif

	#if USE_TRANSPOSITION_TABLES
	int alphaOrig = alpha;
	Minmax::TableEntry* entry = this->getTTEntry(_gomoku.hashState());
	if (entry && entry->depth >= depth) {
		std::cout << "FOUND Entry Depth: " << entry->depth << " vs " << depth << std::endl;
		TTCount++;
		if (entry->flag == Minmax::EntryFlag::Exact) {
			std::cout << "Minmax::EntryFlag::Exact" << std::endl;
			if (root && entry->value > _bestValue) {
				_bestValue = entry->value;
				_bestMove = _gomoku.lastMoves[_gomoku.currentPlayer->getIndex()];
			}
			return entry->value;
		} else if (entry->flag == Minmax::EntryFlag::Lowerbound) {
			std::cout << "Minmax::EntryFlag::Lowerbound" << std::endl;
			alpha = std::max(alpha, entry->value); 
		} else if (entry->flag == Minmax::EntryFlag::Upperbound) {
			std::cout << "Minmax::EntryFlag::Upperbound" << std::endl;
			beta = std::min(beta, entry->value); 
		}

		if (alpha >= beta) {
			std::cout << "Breaking thanks to transpositionTables" << std::endl;
			return entry->value;
		}
	}
	#endif


	if (_gomoku.getEndState() != Gomoku::PLAYING) {
		if (_gomoku.getEndState() >= 0) {
			if (maximizing) {
				return Minmax::LOSS + (_maxDepth - depth);
			} else {
				return Minmax::VICTORY - (_maxDepth - depth);
			}
		} else {
			return 0;
		}
	}

	if (depth == 0) {
		return heuristicValue;
	}


	auto moves = _gomoku.getMoves();
	auto heuristicsByMove = _getSortedMoves(moves, maximizing, depth);

	int value;
	if (maximizing) {
		value = Minmax::LOSS - 1;
		
		#if DEBUG_POS
		_bestValue[depth] = value;
		#endif

		for (auto& heuristicByMove: heuristicsByMove) {
			auto undoMoves = _gomoku.doMove(heuristicByMove.move);
			int ret = _minmaxAlphaBeta(depth - 1, alpha, beta, false, false, heuristicByMove.heuristic);
			_gomoku.undoMove(undoMoves);
			if (_gomoku.shouldReset()) {
				return -1;
			}
			
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
				for (auto lastMove : _gomoku.lastMoves) {
					if (lastMove.first != -1) {
						if (std::abs(heuristicByMove.move.first - lastMove.first) == 1 && std::abs(heuristicByMove.move.second - lastMove.second) == 1) {
							ret += Minmax::PROXIMITY_BONUNS;
							break;
						}
					}
				}
				#endif

				#if RANDOM_POS
				if (ret == _bestValue && std::rand() % 4 == 0) {
					_bestValue = ret;
					_bestMove = heuristicByMove.move;
				}
				#endif
				
				if (ret > _bestValue) {
					_bestValue = ret;
					_bestMove = heuristicByMove.move;
					
					#if DEBUG_POS
					_bestValue[depth] = ret;
					_bestMove[depth] = heuristicByMove.move;
					#endif
				}
				_heuristicValues[heuristicByMove.move.first][heuristicByMove.move.second] = ret;
			}

			if (alpha >= beta) {
				#if PRUNNING
				break;
				#endif
			}
		}

		#if DEBUG_POS
		if (shouldDisplay(_gomoku, depth)) displayDebug(_gomoku, maximizing, alpha, beta, depth);
		#endif
			
	} else {
		value = Minmax::VICTORY + 1;

		#if DEBUG_POS
		_bestValue[depth] = value;
		#endif

		for (auto& heuristicByMove: heuristicsByMove) {
			auto undoMoves = _gomoku.doMove(heuristicByMove.move);
			int ret = _minmaxAlphaBeta(depth - 1, alpha, beta, true, false, heuristicByMove.heuristic);
			_gomoku.undoMove(undoMoves);
			if (_gomoku.shouldReset()) {
				std::cout << "hasBeenReset in minmax" << std::endl;
				return -1;
			}

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
		if (shouldDisplay(_gomoku, depth)) displayDebug(_gomoku, maximizing, alpha, beta, depth);
		#endif

	}

	#if USE_TRANSPOSITION_TABLES
	if (value <= alphaOrig) {
		// std::cout << "Adding entry Upperbound at depth " << depth << std::endl;
		this->addTTEntry(_gomoku.hashState(), value, Minmax::EntryFlag::Upperbound, depth);
	} else if (value >= beta) {
		// std::cout << "Adding entry Lowerbound at depth " << depth << std::endl;
		this->addTTEntry(_gomoku.hashState(), value, Minmax::EntryFlag::Lowerbound, depth);
	} else {
		// std::cout << "Adding entry exactbound at depth " << depth << std::endl;
		this->addTTEntry(_gomoku.hashState(), value, Minmax::EntryFlag::Exact, depth);
	}
	#endif
	return value;
}

std::vector<Minmax::HeuristicByMove> Minmax::_getSortedMoves(std::vector<std::pair<int, int>>& moves, bool maximizing, int depth) {
	std::vector<Minmax::HeuristicByMove> heuristicsByMove;

	std::shared_ptr<Player> currentPlayer = _gomoku.currentPlayer;
	std::shared_ptr<Player> otherPlayer = _gomoku.players[_gomoku.currentPlayer->getIndex() == 0 ? 1 : 0];


	for (auto& move: moves) {
		auto undoMoves = _gomoku.doMove(move);
		if (_gomoku.getEndState() >= 0) {
			if (maximizing) {
				heuristicsByMove.push_back( {
					move,
					Minmax::VICTORY - (_maxDepth - depth),
				});

			} else {
				heuristicsByMove.push_back({
					move,
					Minmax::LOSS + (_maxDepth - depth),
				});
			}
		} else if (_gomoku.getEndState() == Gomoku::State::DRAW) {
			heuristicsByMove.push_back({move, 0});
		} else {
			Minmax::HeuristicByMove hbm;
			hbm.heuristic = _gomoku.heuristic(depth);
			hbm.move = move;
			heuristicsByMove.push_back(hbm);
		}

		_gomoku.undoMove(undoMoves);
	}

	std::vector<Minmax::HeuristicByMove> sortedHeuristicByMove;
	if (maximizing) {
		while (heuristicsByMove.size()) {
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
		while (heuristicsByMove.size()) {
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

// Getter and Setters
Minmax::TableEntry*	Minmax::getTTEntry(std::vector<char> hash) {
	auto ret = _TT.find(hash);
	if (ret != _TT.end()) {
		return &(ret->second);
	} else {
		return nullptr;
	}
}

void		Minmax::addTTEntry(std::vector<char> hash, char value, EntryFlag flag, int depth) {
	_TT[hash] = {
		value,
		flag,
		depth
	};
}

const int Minmax::VICTORY = 100'000'000;
const int Minmax::LOSS = -Minmax::VICTORY;
const int Minmax::CERTAIN_VICTORY = 10'000;
const int Minmax::PROXIMITY_BONUNS = 3;