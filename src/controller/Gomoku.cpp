#include "Gomoku.hpp"

Gomoku::Gomoku(int size, Player::Type player0Type, Player::Type player1Type): size(size), playing(false), remainingStones(size * size), endState(State::PLAYING), winStreakLength(5), turn(0) {
	this->players.push_back(std::make_shared<Player>(0, player0Type));
	this->players.push_back(std::make_shared<Player>(1, player1Type));

	this->board = std::vector<std::vector<int>>(size, std::vector<int>(size, -1));

	this->currentPlayer = this->players[0];
	this->heuristicPlayer = nullptr;
	this->lastMoves = std::vector<std::pair<int, int>>(2, std::make_pair<int, int>(-1, -1));


	_rules = {
		std::make_shared<NoDoubleFreeThree>(),
		std::make_shared<Captures>()
	};

	if (false) {
		_gameEndingCapture = std::make_shared<GameEndingCapture>();
		_rules.push_back(_gameEndingCapture);
	}
}

void Gomoku::reset() {
	this->board = std::vector<std::vector<int>>(size, std::vector<int>(size, -1));
	this->currentPlayer = this->players[0];
	this->heuristicPlayer = nullptr;
	this->lastMoves = std::vector<std::pair<int, int>>(2, std::make_pair<int, int>(-1, -1));
	this->endState = State::PLAYING;
	this->remainingStones = this->size * this->size;
	this->playing = false;
	this->turn = 0;
	this->players[0]->reset();
	this->players[1]->reset();
	
	if (this->minmax->isRunning()) {
		_hasBeenReset = true;
	}
}

void Gomoku::nextTurn() {
	++this->turn;
}

int Gomoku::getTurn() const {
	return this->turn;
}

bool Gomoku::hasBeenReset() const {
	return _hasBeenReset;
}

void Gomoku::clearReset() {
	this->_hasBeenReset = false;
}

std::vector<std::vector<std::pair<int, int>>> fourDirections = {
	{ std::make_pair( 0, 1 ), std::make_pair( 0, -1 ) },
	{ std::make_pair( 1, 0 ), std::make_pair(-1,  0 ) },
	{ std::make_pair( 1, 1 ), std::make_pair(-1, -1 ) },
	{ std::make_pair(-1, 1 ), std::make_pair( 1, -1 ) },
};

int Gomoku::checkWinCondition(std::pair<int, int> pos, int playerIndex) {

	if (this->remainingStones <= 0) {
		return State::DRAW;
	}

	for (auto rule: _rules) {
		int state = rule->checkEndGame(*this);
		if (state != State::PLAYING) {
			return state;
		}
	}

	int otherPlayer = playerIndex == 0 ? 1 : 0;
	int otherPlayerCaptures = this->players[otherPlayer]->getCaptures();

	if (_gameEndingCapture && otherPlayerCaptures == 4) {
		return State::PLAYING;
	}
		
	int fiveAlignedWinner = -1;

	for (std::vector<std::pair<int, int>>& line: fourDirections) {
		int numAligned = 1;

		for (int i = 1; i < 5; i++) {
			std::pair<int, int> nextPos = std::make_pair(pos.first + line[0].first * i, pos.second + line[0].second * i);
			if (nextPos.first >= this->size || nextPos.first < 0 ||
				nextPos.second >= this->size || nextPos.second < 0 ||
				this->board[nextPos.first][nextPos.second] != playerIndex) {
					break ;
			} else {
				numAligned++;
				if (numAligned >= 5) {

					if (_gameEndingCapture) {
						int unbrokenCount = this->canBreakFiveAligned(nextPos, playerIndex, otherPlayer, line);
						std::cout << "[checkWinCondition] after can break 5 aligned (1) unbrokenCount: " << unbrokenCount << std::endl;
						if (unbrokenCount != -1) {
							break ;
						}
						numAligned = unbrokenCount; // ????
						fiveAlignedWinner = playerIndex;
					} else {
						return playerIndex;
					}
				}
			}
		}

		for (int i = 1; i < 5; i++) {
			std::pair<int, int> nextPos = std::make_pair(pos.first + line[1].first * i, pos.second + line[1].second * i);
			if (nextPos.first >= this->size || nextPos.first < 0 ||
				nextPos.second >= this->size || nextPos.second < 0 ||
				this->board[nextPos.first][nextPos.second] != playerIndex) {
					break ;
			} else {

				numAligned++;
				if (numAligned >= 5) {
					
					if (_gameEndingCapture) {
						int unbrokenCount = this->canBreakFiveAligned(nextPos, playerIndex, otherPlayer, line);
						std::cout << "[checkWinCondition] after can break 5 aligned (2) unbrokenCount: " << unbrokenCount << std::endl;
						if (unbrokenCount != -1) {
							break ;
						}
						numAligned = unbrokenCount; // ????
						fiveAlignedWinner = playerIndex;
					} else {
						return playerIndex;
					}

				}
			}
		}
	}

	if (fiveAlignedWinner != -1) {
		return fiveAlignedWinner;
	} else {
		return State::PLAYING;
	}
}

// returns -1 if no break possible
// returns number of stones not broken by break
int Gomoku::canBreakFiveAligned(std::pair<int, int> pos, int playerIndex, int otherPlayer, std::vector<std::pair<int, int>>& line) const {
	std::cout << "[canBreakFiveAligned] " << pos << std::endl;
	for (int i = 0; i < 5; i++) {
		std::pair<int, int> nextPos = std::make_pair(pos.first + line[0].first * i, pos.second + line[0].second * i);
		std::cout << "\tchecking at " << nextPos << " (i = " << i << ")" << std::endl;
		if (nextPos.first >= this->size || nextPos.first < 0 ||
			nextPos.second >= this->size || nextPos.second < 0 ||
			this->board[nextPos.first][nextPos.second] != playerIndex) {
				break ;
		} else {
			if (this->canBreakAtPos(nextPos, playerIndex, otherPlayer)) {
				std::cout << "\tcanBreakFiveAligned (1) returns " << i << std::endl;
				return i;
			}
		}
	}
	for (int i = 1; i < 5; i++) {
		std::pair<int, int> nextPos = std::make_pair(pos.first + line[1].first * i, pos.second + line[1].second * i);
		if (nextPos.first >= this->size || nextPos.first < 0 ||
			nextPos.second >= this->size || nextPos.second < 0 ||
			this->board[nextPos.first][nextPos.second] != playerIndex) {
				break ;
		} else {
			if (this->canBreakAtPos(nextPos, playerIndex, otherPlayer)) {
				std::cout << "\tcanBreakFiveAligned (2) returns " << i << std::endl;
				return i;
			}
		}
	}
	return -1;
}

bool Gomoku::canBreakAtPos(std::pair<int, int> pos, int playerIndex, int otherPlayer) const {
	std::cout << "[canBreakAtPos] " << pos << std::endl;
	for (std::vector<std::pair<int, int>>& line: fourDirections) {
		for (std::pair<int, int>& dir : line) {
			if (pos.first + dir.first < 0 || pos.first + dir.first >= this->size ||
				pos.second + dir.second < 0 || pos.second + dir.second >= this->size ||
				this->board[pos.first + dir.first][pos.second + dir.second] == playerIndex) {
				
				if (pos.first - dir.first < 0 || pos.first - dir.first >= this->size ||
					pos.second - dir.second < 0 || pos.second - dir.second >= this->size ||
					pos.first + dir.first * 2 < 0 || pos.first + dir.first * 2 >= this->size ||
					pos.second + dir.second * 2 < 0 || pos.second + dir.second * 2 >= this->size) {
						continue;
				}

				if ((this->board[pos.first - dir.first][pos.second - dir.second] == -1 && 
					this->board[pos.first + dir.first * 2][pos.second + dir.second * 2] == otherPlayer) ||
					(this->board[pos.first - dir.first][pos.second - dir.second] == otherPlayer && 
					this->board[pos.first + dir.first * 2][pos.second + dir.second * 2] == -1)) {
						std::cout << "\tCan Break !" << std::endl;
						return true;
				}
			}
		}
	}
	return false;
}


void Gomoku::updateBoardCallbacks(std::pair<int, int> pos, int value) {
	if (minmax->isRunning()) {
		return ;
	}
	for (auto& callback: _updateBoardCallbacks) {
		callback(pos, value);
	}
}

void Gomoku::captureCallbacks(int playerIndex) {
	if (minmax->isRunning()) {
		return ;
	}
	for (auto& callback: _captureCallbacks) {
		callback(playerIndex, players[playerIndex]->getCaptures());
	}
}

void Gomoku::onUpdateBoard(std::function<void(std::pair<int, int>, int)> f) {
	_updateBoardCallbacks.push_back(f);
}

void Gomoku::onCapture(std::function<void(int playerIndex, int value)> f) {
	_captureCallbacks.push_back(f);
}

std::vector<AAction*> Gomoku::place(int y, int x, int playerIndex) {
	std::vector<AAction*> actions;

	std::pair<int, int> pos = std::make_pair(y, x);

	this->board[y][x] = playerIndex;

	actions.push_back(new ActionUpdateBoard(pos, -1));

	this->updateBoardCallbacks(pos, playerIndex);

	this->remainingStones--;

	// trigger rule effects
	for (auto rule : _rules) {
		std::vector<AAction*> ruleActions = rule->triggerEffects(*this, pos);
		actions.insert(actions.end(), ruleActions.begin(), ruleActions.end());
	}

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

	// int currentPlayer = this->currentPlayer->index;

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
	// 	std::cout <<
	// 		" currentStreakNum: " << currentStreakNum << 
	// 		" currentStreakPotential: " << currentStreakPotential << 
	// 		" halfOpen: " << halfOpen << 
	// 		" emptyCellCount: " << emptyCellCount << std::endl;
	// }
	if (currentStreakPotential < 5) {
		return 0;
	}

	if (currentStreakNum >= 5 && emptyCellCount == 0) {
		std::cout << "[evalStreakScore] found five aligned" << std::endl;

		// if (this->_gameEndingCapture) {
		// 	std::cout << "[evalStreakScore] \thas game ending capture rule" << std::endl;
		// 	if (this->players[player == 0 ? 1 : 0]->getCaptures() == 4) {
		// 		std::cout << "[evalStreakScore] \tother player [" << (player == 0 ? "1" : "0") << "] has 4 captures! => 5 aligned does not count as victory" << std::endl;
		// 	} else {
		// 		std::cout << "[evalStreakScore] \t5 aligned counts as victory!" << std::endl;
		// 		this->endState = player;
		// 		return Minmax::INF_MAX;
		// 	}
		// } else {
			std::cout << "[evalStreakScore] SHOULD NOT BE HERE" << std::endl;
			return Minmax::INF_MAX / 2;
		// }
	}

	if (player == this->currentPlayer->getIndex()) { // YOUR TURN
		// you win moar
		if (currentStreakNum == 4) {
			return Minmax::CERTAIN_VICTORY;
		}
		if (currentStreakNum == 3 && currentStreakPotential > 5 && !halfOpen && emptyCellCount < 2) {
			return Minmax::CERTAIN_VICTORY / 3;
		}

	} else {
		if (currentStreakNum == 4 && !halfOpen && emptyCellCount == 0) {
			return Minmax::CERTAIN_VICTORY / 2;
		}
	}

	int ret = static_cast<int>(pow((currentStreakNum - emptyCellCount), 3) * (halfOpen ? 1 : 2)) + currentStreakNum;  // give pat medal
	// std::cout << "evalStreakScore: " << ret << std::endl;
	return ret;

	// if (player == this->heuristicPlayer->index) {
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
	(void)rollOverEmptyCells;
	// std::cout << "  resetStreak. rollOverEmptyCells: " << std::boolalpha << rollOverEmptyCells << std::endl;
	streaking = false;
	frontBlocked = false;
	currentStreakNum = 0;
	emptyCellCountPotential = 0;
	emptyCellCount = currentStreakNum;
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

int Gomoku::evalLine(std::pair<int, int> start, std::pair<int, int> line, int player, int length, int& potentialCaptures) {
	// std::cout << "Start: " << start << ", line: " << line << ", size: " << length << ", player: " << player << std::endl;

	int score = 0;
	bool streaking;
	bool frontBlocked;
	int currentStreakNum;
	int currentStreakPotential = 0;
	int emptyCellCount; // number of empty cells in a streak in between player's stones
	int emptyCellCountPotential;
	int potentialCapturesPatternPosition[2] = {0, 0};

	resetStreak(streaking, frontBlocked, currentStreakNum, emptyCellCount, emptyCellCountPotential, false);

	int otherPlayer = player == 0 ? 1 : 0;

	std::pair<int, int> pos;

	for (int i = 0; i < length; i++) {
		pos = std::make_pair<int, int>(start.first + line.first * i, start.second + line.second * i);
				
		if (potentialCapturesPatternPosition[0] == 4) {
			++potentialCaptures;
			potentialCapturesPatternPosition[0] = 0;
		}
		if (board[pos.first][pos.second] == PotentialCapturePattern[player][0][potentialCapturesPatternPosition[0]]) {
			++potentialCapturesPatternPosition[0];
		} else {
			potentialCapturesPatternPosition[0] = 0;
		}
		if (potentialCapturesPatternPosition[1] == 4) {
			++potentialCaptures;
			potentialCapturesPatternPosition[1] = 0;
		}
		if (board[pos.first][pos.second] == PotentialCapturePattern[player][1][potentialCapturesPatternPosition[1]]) {
			++potentialCapturesPatternPosition[1];
		} else {
			potentialCapturesPatternPosition[1] = 0;
		}
		

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

	int potentialCaptures = 0;

	// this->printBoard();

	//auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < this->size; i++) {
		score += this->evalLine(std::make_pair(i, 0), hLine, player, this->size, potentialCaptures);
		if (this->endState != State::PLAYING) {
			// std::cout << "BREAK 1" << std::endl;
			return score;
		}
		score += this->evalLine(std::make_pair(0, i), vLine, player, this->size, potentialCaptures);
		if (this->endState != State::PLAYING) {
			// std::cout << "BREAK 2" << std::endl;
			return score;
		}
	}
	//auto end = std::chrono::steady_clock::now();
	//std::cout << std::chrono::duration <double, std::milli>(end - start).count() << " ms" << std::endl;

	for (int i = 0; i <= this->size - this->winStreakLength; i++) {
		score += this->evalLine(std::make_pair(0, i), dLine1, player, this->size - i, potentialCaptures);
		if (this->endState != State::PLAYING) {
			// std::cout << "BREAK 3" << std::endl;
			return score;
		}
		if (i != 0) {
			score += this->evalLine(std::make_pair(i, 0), dLine1, player, this->size - i, potentialCaptures);
		}
		if (this->endState != State::PLAYING) {
			// std::cout << "BREAK 3" << std::endl;
			return score;
		}
	}

	for (int i = this->size - 1; i >= this->winStreakLength - 1; i--) {
		score += this->evalLine(std::make_pair(0, i), dLine2, player, i + 1, potentialCaptures);
		if (this->endState != State::PLAYING) {
			// std::cout << "BREAK 4" << std::endl;
			return score;
		}
	}

	for (int i = 1; i <= this->size - this->winStreakLength; i++) {
		score += this->evalLine(std::make_pair(i, this->size - 1), dLine2, player, this->size - i, potentialCaptures);
		if (this->endState != State::PLAYING) {
			// std::cout << "BREAK 5" << std::endl;
			return score;
		}
	}
	int otherPlayer = player == 0 ? 1 : 0;

	if (board[1][2] == otherPlayer && board[2][1] == otherPlayer) {
		if ((board[0][3] == -1 && board[3][0] == player) || (board[0][3] == player && board[3][0] == -1)) {
			++potentialCaptures;
		}
	}
	if (board[1][size - 3] == otherPlayer && board[2][size - 2] == otherPlayer) {
		if ((board[0][size - 4] == -1 && board[3][size - 1] == player) || (board[0][size - 4] == player && board[3][size - 1] == -1)) {
			++potentialCaptures;
		}
	}
	if (board[size - 3][1] == otherPlayer && board[size - 2][2] == otherPlayer) {
		if ((board[size - 4][0] == -1 && board[size - 1][4] == player) || (board[size - 4][0] == player && board[size - 1][4] == -1)) {
			++potentialCaptures;
		}
	}
	if (board[size - 2][size - 3] == otherPlayer && board[size - 3][size - 2] == otherPlayer) {
		if ((board[size - 1][size - 4] == -1 && board[size - 4][size - 1] == player) || (board[size - 1][size - 4] == player && board[size - 4][size - 1] == -1)) {
			++potentialCaptures;
		}
	}

	int captures = this->players[player]->getCaptures();
	if (captures >= 5) {
		std::cout << "LOL MDR SHOULD NOT DU TOUT BE HERE" << std::endl;
		return Minmax::INF_MAX / 2;
	}
	if (player == this->currentPlayer->getIndex()) { // YOUR TURN
		// you win moar
		if (captures == 4 && potentialCaptures > 0) {
			return Minmax::CERTAIN_VICTORY;
		}
	}
	// else {
	// 	if (captures == 4 && potentialCaptures > 0) {
	// 		return Minmax::CERTAIN_VICTORY / 2;
	// 	}
	// }

	int capturesValue = std::pow(captures + 1, 3);
	// int potentialCapturesValue = capturesValue * potentialCaptures;

	// std::cout << "potentialCaptures: " << potentialCaptures << ", capturesValue: " << capturesValue << ", potentialCapturesValue: " << potentialCapturesValue << std::endl;

	// return capturesValue + potentialCapturesValue;
	return score;// + capturesValue + potentialCapturesValue;
}

//float hPlayerMultiplier = 1;

int Gomoku::heuristic() {

	int score0 = this->heuristicByPlayer(0);
	int score1 = this->heuristicByPlayer(1);

	//return this->heuristicPlayer->getIndex() == 0 ? hPlayerMultiplier * score0 - score1 : hPlayerMultiplier * score1 - score0;
	return this->heuristicPlayer->getIndex() == 0 ? score0 - score1 : score1 - score0;

	// return this->heuristicPlayer->index == 0 ? score0 : score1;
	// return finalScore;
}


std::vector<AAction*> Gomoku::doMove(std::pair<int, int>& pos) {
	std::vector<AAction*> actions = this->place(pos.first, pos.second, this->currentPlayer->getIndex());
	this->switchPlayer();
	return actions;
}

bool Gomoku::canPlace(std::pair<int, int> move) const {
	for (auto rule: _rules) {
		// std::cout << rule->name << std::endl;
		if (!rule->canPlace(*this, move)) {
			return false;
		}
	}
	return true;
}


void Gomoku::undoMove(std::vector<AAction*>& actions) {
	this->switchPlayer();
	
	for (AAction* action: actions) {
		action->execute(*this);
		delete action;
	}
}

void Gomoku::printBoard() {
	for (int j = 0; j < this->size; j++) {
		for (int i = 0; i < this->size; i++) {
			std::cout << std::setw(6) << (this->board[j][i] == -1 ? "." : std::to_string(this->board[j][i])) << " ";
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
				std::cout << std::setw(6) << (board[j][i] == -1 ? "." : value) << " ";
			}
		}
		
		std::cout << std::endl;
	}
}

void Gomoku::printState() {
	std::cout << "====================== STATE ====================== " << std::endl;
	std::cout << "Size: " << this->size << std::endl;
	std::cout << "Current Player Index: " << this->currentPlayer->getIndex() << std::endl;
	// std::cout << "Heuristic Player Index: " << this->heuristicPlayer->index << std::endl;
	// std::cout << "Last Moves: " << this->lastMoves[this->lastMoves.size() - 1].first << ", " << this->lastMoves[this->lastMoves.size() - 1].second << std::endl;
	std::cout << "EndState: " << this->endState << std::endl;
	std::cout << "=================================================== " << std::endl;
}

void printLineScore(std::vector<int> line, int size, int score, int potentialCaptures) {
	// std::cout << "==============";
	for (int i = 0; i < size; i ++) {
		if (i == size - 1) {
			std::cout << line[i];
		} else {
			std::cout << line[i] << ", ";
		}
	}
	std::cout << "\t => score: " << score << ", potentialCaptures: " << potentialCaptures << std::endl;
}

const int Gomoku::PotentialCapturePattern[][2][4] = {
	{ {0, 1, 1, -1}, {-1, 1, 1, 0} },
	{ {1, 0, 0, -1}, { -1, 0, 0, 1} },
};