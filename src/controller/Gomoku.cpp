#include "Gomoku.hpp"

bool logg = false;

// std::ostream & operator << (std::ostream &out, std::pair<int, int> &c) {
//     out << "[" << c.first << ", " << c.second << "]";
//     return out;
// }


Gomoku::Gomoku(int size, Player::Type player0Type, Player::Type player1Type): size(size), playing(false), remainingStones(size * size), endState(State::PLAYING), winStreakLength(5), turn(0) {
	this->players.push_back(std::make_shared<Player>(0, player0Type));
	this->players.push_back(std::make_shared<Player>(1, player1Type));

	this->board = std::vector<std::vector<int>>(size, std::vector<int>(size, -1));

	this->currentPlayer = this->players[0];
	this->heuristicPlayer = nullptr;
	// this->lastMoves = std::vector<std::pair<int, int>>(2, std::make_pair<int, int>(-1, -1));

	this->rules.push_back(new NoDoubleFreeThree());
}

void Gomoku::reset() {
	this->board = std::vector<std::vector<int>>(size, std::vector<int>(size, -1));
	this->currentPlayer = this->players[0];
	this->heuristicPlayer = nullptr;
	// this->lastMoves = std::vector<std::pair<int, int>>(2, std::make_pair<int, int>(-1, -1));
	this->endState = State::PLAYING;
	this->remainingStones = this->size * this->size;
	this->playing = false;
	this->turn = 0;
}

void Gomoku::nextTurn() {
	++this->turn;
}

int Gomoku::getTurn() const {
	return this->turn;
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

	// std::cout << "==== THEN ==== " << std::endl;
	// this->gomoku->printBoard(this->gomoku->board, pos);
	this->board[y][x] = playerIndex;
	// std::cout << "===== NOW ==== " << std::endl;
	// this->gomoku->printBoard(this->gomoku->board, pos);
	// this->lastMoves[playerIndex] = pos;

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
	if (this->currentPlayer == this->players[0]) {
		this->currentPlayer = this->players[1];
	} else {
		this->currentPlayer = this->players[0];
	}
}

std::vector<std::pair<int, int>> Gomoku::getMoves() {
	std::vector<std::pair<int, int>> moves;

	// int distAroundLastMoves = 1;

	int currentPlayer = this->currentPlayer->index;

	bool empty = true; 

	// for (int i = (currentPlayer == 0 ? 0 : 1); i != (currentPlayer == 0 ? 2 : -1); i += (currentPlayer == 0 ? 1 : -1)) {
	// 	auto move = this->lastMoves[i];
		
	// 	if (move.first != -1) {
	// 		for (int i = -distAroundLastMoves; i <= distAroundLastMoves; i++) {
	// 			int i2 = i + move.first;
	// 			for (int j = -distAroundLastMoves; j <= distAroundLastMoves; j++) {
	// 				int j2 = j + move.second;
	// 				if (i2 >= 0 && i2 < this->size && j2 >= 0 && j2 < this->size && this->board[i2][j2] == -1) {
	// 					auto child = std::make_pair(i2, j2);
	// 					if (std::find(moves.begin(), moves.end(), child) == moves.end()) {
	// 						moves.push_back(child);
	// 						empty = false;
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// }

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
			// std::cout << "Removed " << it->first << ", " << it->second << std::endl;
			erased = true;
			it = moves.erase(it);
		}
		if (!erased) {
			it++;
		}
	}
	return moves;
}

int Gomoku::evalStreakScore(int currentStreakNum, int currentStreakPotential, bool halfOpen, int player, int emptyCellCount) {
	// if (logg) {
		// std::cout <<
		// 	" currentStreakNum: " << currentStreakNum << 
		// 	" currentStreakPotential: " << currentStreakPotential << 
		// 	" halfOpen: " << halfOpen << 
		// 	" emptyCellCount: " << emptyCellCount << std::endl;
	// }
	if (currentStreakPotential < 5) {
		return 0;
	}

	// if (player == this->heuristicPlayer->index) {
		// Look for Minmax::victory conditions
		if (currentStreakNum >= 5 && emptyCellCount == 0) {
			std::cout << "SHOULD NOT BE HERE" << std::endl;
			return Minmax::CERTAIN_VICTORY;
		}
		if (currentStreakNum == 4 && !halfOpen && emptyCellCount == 0) {
			return Minmax::CERTAIN_VICTORY;
		}
		int ret = static_cast<int>(pow((currentStreakNum - emptyCellCount), 3) * (halfOpen ? 1 : 2)) + currentStreakNum;  // give pat medal
		// std::cout << "evalStreakScore: " << ret << std::endl;
		return ret;
	// } else {
		// Look for threats
		// if (currentStreakNum >= 5 && emptyCellCount == 0) {
		// 	std::cout << "SHOULD NOT BE HERE" << std::endl;
		// 	return Minmax::CERTAIN_VICTORY * 10;
		// }
		// if (currentStreakNum == 4) {
		// 	return Minmax::CERTAIN_VICTORY * 10;
		// }
		// if (currentStreakNum == 3 && !halfOpen) {
		// 	return Minmax::CERTAIN_VICTORY * 10;
		// }
		// return 0;
	// }

	return 0;
}

void resetStreak(bool &streaking, bool &frontBlocked, int &currentStreakNum, int& emptyCellCount, int& emptyCellCountPotential, bool rollOverEmptyCells) {
	
	// std::cout << "  resetStreak. rollOverEmptyCells: " << std::boolalpha << rollOverEmptyCells << std::endl;
	streaking = false;
	frontBlocked = false;
	currentStreakNum = 0;
	// currentStreakPotential = rollOverEmptyCells ? emptyCellCount : 0;
	emptyCellCountPotential = 0;
	emptyCellCount = currentStreakNum;
	// std::cout << "frontBlocked: " << frontBlocked << std::endl;
}


bool Gomoku::hasEnoughPotential(std::pair<int, int> start, std::pair<int, int> line, int countToCheck, int otherPlayer) {
	auto pos = start;
	while (countToCheck--) {
		pos = std::make_pair<int, int>(pos.first + line.first, pos.second + line.second);

		if (pos.first < 0 || pos.second < 0 || pos.first >= this->size || pos.second >= this->size || this->board[pos.first][pos.second] == otherPlayer) {
			return false;
		}
	}
	return true;
}

int Gomoku::evalLine(std::pair<int, int> start, std::pair<int, int> line, int player, int length) {
	// logg = start.first == 1 && line.second == 1;
	// if (logg) {
		// std::cout << "Eval line: start: " << start << std::endl;
	// }

	// std::cout << "Start: " << start << ", line: " << line << ", size: " << length << ", player: " << player << std::endl;

	int score = 0;
	bool streaking;
	bool frontBlocked;
	int currentStreakNum;
	int currentStreakPotential = 0;
	int emptyCellCount; // number of empty cells in a streak in between player's stones
	int emptyCellCountPotential;

	resetStreak(streaking, frontBlocked, currentStreakNum, emptyCellCount, emptyCellCountPotential, false);

	int otherPlayer = player == 0 ? 1 : 0;

	std::pair<int, int> pos;

	for (int i = 0; i < length; i++) {
		pos = std::make_pair<int, int>(start.first + line.first * i, start.second + line.second * i);
		// if (logg) {
			// std::cout << "pos: " << pos << " is " << this->board[pos.first][pos.second] << std::endl;
		// }

		// if player
		if (this->board[pos.first][pos.second] == player) {
			// if not streaking
			if (!streaking) {
				// if currentStreakNum is not 0
				if (currentStreakNum) {
					// std::cout << "evalStreakScore0: " << std::endl;
					if (currentStreakPotential < 5) {
						if (this->hasEnoughPotential(pos, line, 5 - currentStreakPotential, otherPlayer)) {
							score += this->evalStreakScore(currentStreakNum, 100, frontBlocked, player, emptyCellCount);
						}
					} else {
						score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked, player, emptyCellCount);
					}
					resetStreak(streaking, frontBlocked, currentStreakNum, emptyCellCount, emptyCellCountPotential, i > 0 && this->board[pos.first - line.first][pos.second - line.second] == -1);
					currentStreakNum = 1;
					// currentStreakPotential = 1;
					// std::cout << "  Reset currentStreakNum: " << currentStreakNum << std::endl;
					// std::cout << "  Reset currentStreakPotential: " << currentStreakPotential << std::endl;
				} else {
					// if currentStreakNum is 0
					currentStreakNum++;
					currentStreakPotential++;
					// std::cout << "  Increase currentStreakNum: " << currentStreakNum << std::endl;
					// std::cout << "  Increase currentStreakPotential: " << currentStreakPotential << std::endl;
				}

				frontBlocked = i == 0 || (i > 0 && this->board[pos.first - line.first][pos.second - line.second] == otherPlayer);
				// std::cout << "frontBlocked: " << frontBlocked << std::endl;
				
				streaking = true;
			} else { // if streaking

				emptyCellCount += emptyCellCountPotential;
				// std::cout << "  Increase emptyCellCount: " << emptyCellCount << std::endl;
				emptyCellCountPotential = 0;

				// std::cout <<
				// 	" streaking: " << streaking << 
				// 	" currentStreakNum: " << currentStreakNum << 
				// 	" currentStreakPotential: " << currentStreakPotential << 
				// 	" emptyCellCount: " << emptyCellCount << 
				// 	" frontBlocked: " << frontBlocked << std::endl;
				// std::cout << "evalStreakScore1: " << std::endl;
				// score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked, player, emptyCellCount);
				// std::cout << "\tscore => " << score << std::endl;
				// resetStreak(streaking, frontBlocked, currentStreakNum, currentStreakPotential, emptyCellCount, i > 0 && this->board[pos.first - line.first][pos.second - line.second] == -1);
				// frontBlocked = i == 0 || (i > 0 && this->board[pos.first - line.first][pos.second - line.second] == otherPlayer);
				// std::cout << "frontBlocked: " << frontBlocked << std::endl;
				
				streaking = true;

				currentStreakPotential++;
				currentStreakNum++;
				// std::cout << "  Increase currentStreakNum: " << currentStreakNum << std::endl;
				// std::cout << "  Increase currentStreakPotential: " << currentStreakPotential << std::endl;
			}
		}
		if (this->board[pos.first][pos.second] == -1) {
			currentStreakPotential++;
			// std::cout << "  Increase currentStreakPotential: " << currentStreakPotential << std::endl;

			if (streaking) {
				emptyCellCountPotential++;
				// std::cout << "  Increase emptyCellCountPotential: " << emptyCellCountPotential << std::endl;

				if (i > 0 && this->board[pos.first - line.first][pos.second - line.second] == -1) {
					streaking = false;
				}
			}
		}

		// if other player
		if (this->board[pos.first][pos.second] == otherPlayer) {
			if (streaking) {
				// std::cout << "evalStreakScore2: " << std::endl;
				score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked || (i > 0 && this->board[pos.first - line.first][pos.second - line.second] != -1), player, emptyCellCount);
			} else {
				// std::cout << "evalStreakScore3: " << std::endl;
				score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked, player, emptyCellCount);
			}
			resetStreak(streaking, frontBlocked, currentStreakNum, emptyCellCount, emptyCellCountPotential, i > 0 && this->board[pos.first - line.first][pos.second - line.second] == -1);
			currentStreakPotential = 0;
			// std::cout << "  Reset currentStreakPotential: " << currentStreakPotential << std::endl;
		}
	}

	if (streaking) {
		// std::cout <<
		// 	" streaking: " << streaking << 
		// 	" currentStreakNum: " << currentStreakNum << 
		// 	" currentStreakPotential: " << currentStreakPotential << 
		// 	" emptyCellCount: " << emptyCellCount << 
		// 	" frontBlocked: " << frontBlocked << std::endl;
		// std::cout << "evalStreakScore4: " << std::endl;
		score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked || this->board[pos.first][pos.second] != -1, player, emptyCellCount);
		// std::cout << "\tscore => " << score << std::endl;
	} else {
		// std::cout <<
		// 	" streaking: " << streaking << 
		// 	" currentStreakNum: " << currentStreakNum << 
		// 	" currentStreakPotential: " << currentStreakPotential << 
		// 	" emptyCellCount: " << emptyCellCount << 
		// 	" frontBlocked: " << frontBlocked << std::endl;
		// std::cout << "evalStreakScore5: " << std::endl;
																				// frontBlocked useful?
		score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked || this->board[pos.first][pos.second] != -1, player, emptyCellCount);
	}

	return score;
}

int Gomoku::heuristicByPlayer(int player) {
	int score = 0;
	std::pair<int, int> hLine = std::make_pair<int, int>(0, 1);
	std::pair<int, int> vLine = std::make_pair<int, int>(1, 0);

	std::pair<int, int> dLine1 = std::make_pair<int, int>(1, 1);
	std::pair<int, int> dLine2 = std::make_pair<int, int>(1, -1);

	// this->printBoard();

	//std::vector<std::future<int>> scores;
	//auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < this->size; i++) {
		
		//scores.push_back(std::async(&Gomoku::evalLine, this, std::make_pair(i, 0), hLine, player, this->size));
		score += this->evalLine(std::make_pair(i, 0), hLine, player, this->size);
		// std::cout << "score1: => " << score << std::endl;
		
		// if (i == 3) {
			score += this->evalLine(std::make_pair(0, i), vLine, player, this->size);
			// std::cout << "score2: => " << score << std::endl;
		// }
	}
	//auto end = std::chrono::steady_clock::now();
	//std::cout << std::chrono::duration <double, std::milli>(end - start).count() << " ms" << std::endl;

	//for (auto it = scores.begin(); it != scores.end(); ++it) {
	//	score += it->get();
	//}

	for (int i = 0; i <= this->size - this->winStreakLength; i++) {
		score += this->evalLine(std::make_pair(0, i), dLine1, player, this->size - i);
		// std::cout << "score3: => " << score << std::endl;
		if (i != 0) {
			score += this->evalLine(std::make_pair(i, 0), dLine1, player, this->size - i);
			// std::cout << "score4: => " << score << std::endl;
		}
	}

	for (int i = this->size - 1; i >= this->winStreakLength - 1; i--) {
		score += this->evalLine(std::make_pair(0, i), dLine2, player, i + 1);
		// std::cout << "score5: => " << score << std::endl;
	}

	for (int i = 1; i <= this->size - this->winStreakLength; i++) {
		score += this->evalLine(std::make_pair(i, this->size - 1), dLine2, player, this->size - i);
		// std::cout << "score6: => " << score << std::endl;
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
		if (!rule->canPlace(*this, move)) {
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

void Gomoku::printBoard(std::vector<std::vector<int>> board, std::pair<int, int> pos) {
	for (int j = 0; j < this->size; j++) {
		for (int i = 0; i < this->size; i++) {
			std::string value;

			if (board[j][i] < Minmax::INF_MIN + 20) {
				value = "m" + std::to_string(std::abs(board[j][i] % 10));
			} else if (board[j][i] > Minmax::INF_MAX - 20) {
				value = "M" + std::to_string(std::abs(board[j][i] % 10));
			} else {
				value = std::to_string(board[j][i]);
			}
			if (j == pos.first && i == pos.second) {
				std::cout << "\033[1;31m" << std::setw(6) << value << "\033[0m" << " ";
			} else {
				std::cout << std::setw(6) << value << " ";
			}
		}
		
		std::cout << std::endl;
	}
}

void Gomoku::printState() {
	std::cout << "====================== STATE ====================== " << std::endl;
	std::cout << "Size: " << this->size << std::endl;
	std::cout << "Current Player Index: " << this->currentPlayer->index << std::endl;
	// std::cout << "Heuristic Player Index: " << this->heuristicPlayer->index << std::endl;
	// std::cout << "Last Moves: " << this->lastMoves[this->lastMoves.size() - 1].first << ", " << this->lastMoves[this->lastMoves.size() - 1].second << std::endl;
	std::cout << "EndState: " << this->endState << std::endl;
	std::cout << "=================================================== " << std::endl;
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

void Gomoku::testEvalLine() {
	std::cout << "testEvalLine" << std::endl;
	Gomoku* gomoku = new Gomoku(10, Player::Type::AI, Player::Type::AI);

	gomoku->board = std::vector<std::vector<int>>();

	std::pair<int, int> dir = std::make_pair(0, 1);

	int player = 0;
	gomoku->heuristicPlayer = gomoku->players[0];
	
	// Streak length tests
	// std::cout << "Streak length tests" << std::endl;
	// {
	// 	int lines[][10] = {
	// 		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	// 		{ -1, 0, -1, -1, -1, -1, -1, -1, -1, -1 },
	// 		{ -1, 0, 0, -1, -1, -1, -1, -1, -1, -1 },
	// 		{ -1, 0, 0, 0, -1, -1, -1, -1, -1, -1 },
	// 		{ -1, 0, 0, 0, 0, -1, -1, -1, -1, -1 },
	// 		{ -1, 0, 0, 0, 0, 0, -1, -1, -1, -1 },
	// 		{ -1, 0, 0, 0, 0, 0, 0, -1, -1, -1 },
	// 		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	// 		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	// 		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	// 	};

	// 	for (auto &line: lines) {
	// 		Gomoku::gomoku->board.push_back(std::vector<int> (std::begin(line), std::end(line)));
	// 	} 
	// 	for (int i = 0; i < 7; i++) {
	// 		int ret = Gomoku::gomoku->evalLine(std::make_pair(i, 0), dir, player, 10);
	// 		printLineScore(Gomoku::gomoku->board[i], 10, ret);
	// 	}
	// }

	// // Half opened tests
	// std::cout << "Half opened tests" << std::endl;
	// Gomoku::gomoku->board.clear();
	// {
	// 	int lines[][10] = {
	// 		{-1, 0, 0, -1, -1, -1, -1, -1, -1, -1 },
	// 		{ 1, 0, 0, -1, -1, -1, -1, -1, -1, -1 },
	// 		{ 0, 0, -1, -1, -1, -1, -1, -1, -1, -1 },

	// 		{-1, 0, 0, 0, -1, -1, -1, -1, -1, -1 },
	// 		{ 1, 0, 0, 0, -1, -1, -1, -1, -1, -1 },
	// 		{ 0, 0, 0, -1, -1, -1, -1, -1, -1, -1 },

	// 		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	// 		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	// 		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	// 		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	// 	};

	// 	for (auto &line: lines) {
	// 		Gomoku::gomoku->board.push_back(std::vector<int> (std::begin(line), std::end(line)));
	// 	} 
	// 	for (int i = 0; i < 6; i++) {
	// 		int ret = Gomoku::gomoku->evalLine(std::make_pair(i, 0), dir, player, 10);
	// 		printLineScore(Gomoku::gomoku->board[i], 10, ret);
	// 	}
	// }
	// // Discontinued tests
	// std::cout << "Discontinued tests" << std::endl;
	// Gomoku::gomoku->board.clear();
	// {
	// 	int lines[][10] = {
	// 		{ -1, 0, 0, -1, -1, -1, -1, -1, -1, -1 },
	// 		{ -1, 0, -1, 0, -1, -1, -1, -1, -1, -1 },

	// 		{ -1, 0, 0, 0, -1, -1, -1, -1, -1, -1 },
	// 		{ -1, 0, 0, -1, 0, -1, -1, -1, -1, -1 },
			
	// 		{ -1, 0, 0, 0, 0, -1, -1, -1, -1, -1 },
	// 		{ -1, 0, 0, -1, 0, 0, -1, -1, -1, -1 },
	// 		{ -1, 0, -1, 0, 0, 0, -1, -1, -1, -1 },
			
	// 		{ -1, 0, 0, 0, 0, 0, -1, -1, -1, -1 },
	// 		{ -1, 0, 0, -1, 0, 0, 0, -1, -1, -1 },
	// 		{ -1, 0, -1, -1, 0, 0, 0, 0, -1, -1 },
	// 	};

	// 	for (auto &line: lines) {
	// 		Gomoku::gomoku->board.push_back(std::vector<int> (std::begin(line), std::end(line)));
	// 	} 
	// 	for (int i = 0; i < 10; i++) {
	// 		int ret = Gomoku::gomoku->evalLine(std::make_pair(i, 0), dir, player, 10);
	// 		printLineScore(Gomoku::gomoku->board[i], 10, ret);
	// 	}
	// }

	gomoku = new Gomoku(7, Player::Type::AI, Player::Type::AI);

	gomoku->board = std::vector<std::vector<int>>();

	gomoku->board.clear();
	{
		int lines[][7] = {
			{ -1,  0, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1,  0, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
		};

		for (auto &line: lines) {
			gomoku->board.push_back(std::vector<int> (std::begin(line), std::end(line)));
		} 
		for (int i = 0; i < 2; i++) {
			int ret = gomoku->evalLine(std::make_pair(i, 0), dir, player, 7);
			printLineScore(gomoku->board[i], 7, ret);
		}
	}
}

void Gomoku::testHeuristic() {
	std::cout << "testHeuristic" << std::endl;
	
	Gomoku* gomoku = new Gomoku(7, Player::Type::AI, Player::Type::AI);

	gomoku->board = std::vector<std::vector<int>>();

	gomoku->currentPlayer = gomoku->players[0];

	// {
	// 	int lines[][7] = {
	// 		{ -1, -1, -1, -1, -1, -1, -1 },
	// 		{ -1, -1, -1, -1, -1, -1,  0 },
	// 		{ -1, -1,  0,  0, -1,  1, -1 },
	// 		{ -1, -1,  0,  0,  1, -1, -1 },
	// 		{ -1,  1, -1,  1,  1, -1, -1 },
	// 		{ -1, -1, -1, -1, -1, -1, -1 },
	// 		{ -1, -1, -1, -1, -1, -1, -1 },
	// 	};

	// 	for (auto &line: lines) {
	// 		gomoku->board.push_back(std::vector<int> (std::begin(line), std::end(line)));
	// 	}

	// 	gomoku->heuristicPlayer = gomoku->currentPlayer;

	// 	auto heuristicValues = std::vector<std::vector<int>>(gomoku->size, std::vector<int>(gomoku->size, 0));
		
	// 	auto moves = gomoku->getMoves();
	// 	for (auto &move: moves) {
	// 		auto undoMoves = gomoku->doMove(move);

	// 		int ret = gomoku->heuristic();
			
	// 		heuristicValues[move.first][move.second] = ret;

	// 		gomoku->undoMove(undoMoves);
	// 	}
	// 	gomoku->printBoard(heuristicValues);
	// 	std::cout << "=======\n";
	// }

	{
		// int lines[][7] = {
		// 	{ -1, -1, -1, -1, -1, -1, -1 },
		// 	{ -1,  0, -1, -1, -1,  0, -1 },
		// 	{ -1, -1, -1, -1, -1, -1, -1 },
		// 	{ -1, -1, -1,  0, -1, -1, -1 },
		// 	{ -1, -1, -1, -1, -1, -1, -1 },
		// 	{ -1,  0, -1, -1, -1,  0, -1 },
		// 	{ -1, -1, -1, -1, -1, -1, -1 },
		// };
		// int lines[][7] = {
		// 	{ -1, -1, -1, -1, -1, -1, -1 },
		// 	{ -1, -1, -1, -1, -1, -1, -1 },
		// 	{ -1, -1, -1, -1, -1, -1, -1 },
		// 	{ -1, -1, -1,  0, -1, -1, -1 },
		// 	{ -1, -1, -1, -1, -1, -1, -1 },
		// 	{ -1, -1, -1, -1, -1, -1, -1 },
		// 	{ -1, -1, -1, -1, -1, -1, -1 },
		// };
		// int lines[][7] = {
		// 	{  0, -1, -1, -1, -1, -1,  0 },
		// 	{ -1,  0, -1, -1, -1,  0, -1 },
		// 	{ -1, -1,  0, -1,  0, -1, -1 },
		// 	{ -1, -1, -1,  0, -1, -1, -1 },
		// 	{ -1, -1,  0, -1,  0, -1, -1 },
		// 	{ -1,  0, -1, -1, -1,  0, -1 },
		// 	{  0, -1, -1, -1, -1, -1,  0 },
		// };
		// int lines[][7] = {
		// 	{  0, -1, -1,  1, -1, -1,  0 },
		// 	{ -1,  0, -1, -1, -1,  0, -1 },
		// 	{ -1, -1,  0, -1,  0, -1, -1 },
		// 	{  1, -1, -1,  0, -1, -1,  1 },
		// 	{ -1, -1,  0, -1,  0, -1, -1 },
		// 	{ -1,  0, -1, -1, -1,  0, -1 },
		// 	{  0, -1, -1,  1, -1, -1,  0 },
		// };
		// int lines[][7] = {
		// 	{  0,  0,  0,  0,  0,  0,  0 },
		// 	{  0, -1, -1, -1, -1, -1,  0 },
		// 	{  0, -1, -1, -1, -1, -1,  0 },
		// 	{  0, -1, -1, -1, -1, -1,  0 },
		// 	{  0, -1, -1, -1, -1, -1,  0 },
		// 	{  0, -1, -1, -1, -1, -1,  0 },
		// 	{  0,  0,  0,  0,  0,  0,  0 },
		// };
		int lines[][7] = {
			{  0,  0,  0,  0,  0,  0,  0 },
			{  0, -1, -1,  1, -1, -1,  0 },
			{  0, -1,  1,  0,  1, -1,  0 },
			{  0, -1, -1,  1, -1, -1,  0 },
			{  0, -1, -1,  0, -1, -1,  0 },
			{  0, -1, -1,  1, -1, -1,  0 },
			{  0,  0,  0,  0,  0,  0,  0 },
		};

		for (auto &line: lines) {
			gomoku->board.push_back(std::vector<int> (std::begin(line), std::end(line)));
		}

		gomoku->heuristicPlayer = gomoku->currentPlayer;

		auto heuristicValues = std::vector<std::vector<int>>(gomoku->size, std::vector<int>(gomoku->size, 0));
		
		auto moves = gomoku->getMoves();
		// std::vector<std::pair<int, int>> moves = {
		// 	std::make_pair(0, 0),
		// 	std::make_pair(6, 6)
		// };
		// std::vector<std::pair<int, int>> moves = {
		// 	std::make_pair(2, 3),
		// 	std::make_pair(4, 3)
		// };
		for (auto &move: moves) {
			// std::cout << "move: [" << move.first << ", " << move.second << "]" << std::endl << std::endl;   
			auto undoMoves = gomoku->doMove(move);

			int ret = gomoku->heuristic();
			
			heuristicValues[move.first][move.second] = ret;

			gomoku->undoMove(undoMoves);
		}
		gomoku->printBoard(heuristicValues);
		std::cout << "=======\n";
	}

}

void Gomoku::testMinmax() {
	std::cout << "testHeuristic" << std::endl;
	
	//Gomoku* gomoku = new Gomoku(7, Player::Type::AI, Player::Type::AI);
	//gomoku->minmax = std::make_shared<Minmax>(*gomoku, 4);

	//gomoku->board = std::vector<std::vector<int>>();
	
	// {
		// gomoku->currentPlayer = &gomoku->players[0];

		// for (int i = 5; i >= 2; i--) {
		// 	gomoku->minmax->maxDepth = i;
		// 	int lines[][7] = {
		// 		{ -1, -1, -1, -1, -1, -1, -1 },
		// 		{ -1, -1, -1, -1, -1, -1,  0 },
		// 		{ -1, -1,  0,  0, -1,  1, -1 },
		// 		{ -1, -1,  0,  0,  1, -1, -1 },
		// 		{ -1,  1, -1,  1,  1, -1, -1 },
		// 		{ -1, -1, -1, -1, -1, -1, -1 },
		// 		{ -1, -1, -1, -1, -1, -1, -1 },
		// 	};

		// 	for (auto &line: lines) {
		// 		gomoku->board.push_back(std::vector<int> (std::begin(line), std::end(line)));
		// 	}

		// 	std::cout << "===== DEPTH " << i  << " ====="<< std::endl;
		// 	auto pos = gomoku->minmax->run();
		// }
	// }
	// {
	// 	gomoku->currentPlayer = &gomoku->players[1];

	// 	for (int i = 5; i >= 2; i--) {
	// 		gomoku->minmax->maxDepth = i;
	// 		int lines[][7] = {
	// 			{ -1, -1, -1, -1, -1, -1, -1 },
	// 			{ -1, -1, -1, -1, -1, -1, -1 },
	// 			{ -1, -1,  0,  1,  1, -1, -1 },
	// 			{ -1, -1,  0,  0,  1, -1, -1 },
	// 			{ -1, -1,  0, -1, -1, -1, -1 },
	// 			{ -1, -1, -1, -1, -1, -1, -1 },
	// 			{ -1, -1, -1, -1, -1, -1, -1 },
	// 		};

	// 		for (auto &line: lines) {
	// 			gomoku->board.push_back(std::vector<int> (std::begin(line), std::end(line)));
	// 		}

	// 		std::cout << "===== DEPTH " << i  << " ====="<< std::endl;
	// 		auto pos = gomoku->minmax->run();
	// 	}
	// }

	// {
	// 	gomoku->currentPlayer = &gomoku->players[1];
	// 	// gomoku->lastMoves.push_back(std::make_pair(3, 2));
	// 	gomoku->printState();
	// 	for (int i = 7; i >= 2; i--) {
	// 		gomoku->minmax->maxDepth = i;
	// 		int lines[][7] = {
	// 			{ -1, -1, -1, -1, -1, -1, -1 },
	// 			{ -1, -1, -1, -1,  0, -1, -1 },
	// 			{ -1, -1,  0,  0,  1, -1, -1 },
	// 			{ -1, -1,  0,  0,  1, -1, -1 },
	// 			{ -1, -1, -1,  1,  1, -1, -1 },
	// 			{ -1, -1, -1, -1,  1, -1, -1 },
	// 			{ -1, -1, -1, -1,  0, -1, -1 },
	// 		};

	// 		for (auto &line: lines) {
	// 			gomoku->board.push_back(std::vfector<int> (std::begin(line), std::end(line)));
	// 		}

	// 		std::cout << "===== DEPTH " << i  << " ====="<< std::endl;
	// 		auto pos = gomoku->minmax->run();
	// 	}
	// }

	//{
	//	gomoku->currentPlayer = gomoku->players[0];
	//	// gomoku->lastMoves.push_back(std::make_pair(3, 3));
	//	gomoku->printState();
	//	for (int i = 5; i >= 2; i--) {
	//		gomoku->minmax->maxDepth = i;
	//		int lines[][7] = {
	//			{ -1, -1, -1, -1, -1, -1, -1 },
	//			{ -1, -1,  1, -1, -1, -1, -1 },
	//			{ -1, -1,  0,  1, -1, -1, -1 },
	//			{ -1, -1,  0,  0,  1, -1, -1 },
	//			{ -1, -1,  0, -1,  1, -1, -1 },
	//			{ -1, -1, -1, -1, -1, -1, -1 },
	//			{ -1, -1, -1, -1, -1, -1, -1 },
	//		};

	//		for (auto &line: lines) {
	//			gomoku->board.push_back(std::vector<int> (std::begin(line), std::end(line)));
	//		}

	//		std::cout << "===== DEPTH " << i  << " ====="<< std::endl;
	//		gomoku->minmax->run();
	//	}
	//}

	{
		Gomoku* gomoku = new Gomoku(19, Player::Type::AI, Player::Type::AI);
		gomoku->minmax = std::make_shared<Minmax>(*gomoku, 4);

		gomoku->board = std::vector<std::vector<int>>();
		gomoku->currentPlayer = gomoku->players[1];
		// gomoku->lastMoves.push_back(std::make_pair(3, 3));
		gomoku->printState();
		for (int i = 4; i >= 2; i--) {
			gomoku->minmax->maxDepth = i;
			int lines[][19] = {
				{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1,  0,  0,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1,  1,  0,  0,  0, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1,  1,  1,  0,  1,  1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1,  1,  1,  1,  0,  1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1,  0,  0,  0,  1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
				{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			};

			for (auto& line : lines) {
				gomoku->board.push_back(std::vector<int>(std::begin(line), std::end(line)));
			}

			std::cout << "===== DEPTH " << i << " =====" << std::endl;
			gomoku->minmax->run();
		}
	}

}