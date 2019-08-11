#include "Gomoku.hpp"

Gomoku::Gomoku(int size, Player::Type player0Type, Player::Type player1Type): size(size), playing(false), remainingStones(size * size), winStreakLength(5), turn(0), _board(std::vector<int>(size * size, -1)), _endState(State::PLAYING) {
	this->players.push_back(std::make_shared<Player>(0, player0Type));
	this->players.push_back(std::make_shared<Player>(1, player1Type));

	this->currentPlayer = this->players[0];
	this->heuristicPlayer = nullptr;
	this->lastMoves = std::vector<std::pair<int, int>>(2, std::make_pair<int, int>(-1, -1));

	_rules = {
		std::make_shared<NoDoubleFreeThree>(),
		std::make_shared<Captures>()
	};

	if (true) {
		_gameEndingCapture = std::make_shared<GameEndingCapture>();
		_rules.push_back(_gameEndingCapture);
	}
}

void Gomoku::reset() {
	this->_board = std::vector<int>(size * size, -1);
	this->_endState = State::PLAYING;
	
	this->currentPlayer = this->players[0];
	this->heuristicPlayer = nullptr;
	this->lastMoves = std::vector<std::pair<int, int>>(2, std::make_pair<int, int>(-1, -1));
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

bool Gomoku::onBoard(std::pair<int, int> &pos) const {
	return pos.first >= 0 &&
		pos.second >= 0 &&
		pos.first < this->size &&
		pos.second < this->size;
}

bool Gomoku::notOnBoard(std::pair<int, int> &pos) const {
	return pos.first < 0 ||
		pos.second < 0 ||
		pos.first >= this->size ||
		pos.second >= this->size;
}

int Gomoku::getValueOnBoard(std::pair<int, int> pos) const {
	return this->_board[pos.first * size + pos.second];
}

int Gomoku::getValueOnBoard(int y, int x) const {
	return this->_board[y * size + x];
}

void Gomoku::setValueOnBoard(std::pair<int, int> pos, int value) {
	this->_board[pos.first * size + pos.second] = value;
}

void Gomoku::setValueOnBoard(int y, int x, int value) {
	this->_board[y * size + x] = value;
}

int Gomoku::getRemainingStones() {
	return this->remainingStones;
}

void Gomoku::setRemainingStones(int i) {
	this->remainingStones = i;
}

void Gomoku::setEndState(int i) {
	_endState = (Gomoku::State)i;
}

void Gomoku::setEndState(Gomoku::State i) {
	_endState = i;
}

Gomoku::State Gomoku::getEndState() {
	return _endState;
}

std::vector<std::vector<std::pair<int, int>>> fourDirections = {
	{ std::make_pair( 0, 1 ), std::make_pair( 0, -1 ) },
	{ std::make_pair( 1, 0 ), std::make_pair(-1,  0 ) },
	{ std::make_pair( 1, 1 ), std::make_pair(-1, -1 ) },
	{ std::make_pair(-1, 1 ), std::make_pair( 1, -1 ) },
};

int Gomoku::checkWinCondition(std::pair<int, int> pos, int currentPlayer) {
	if (this->remainingStones <= 0) {
		return State::DRAW;
	}

	for (auto rule: _rules) {
		if (rule == _gameEndingCapture) {
			continue;
		}
		int state = rule->checkEndGame(*this);
		if (state != State::PLAYING) {
			return state;
		}
	}

	for (std::vector<std::pair<int, int>>& line: fourDirections) {
		int numAligned = 1;
		std::pair<int, int> endPos = pos;

		for (int i = 1; i < 5; ++i) {
			std::pair<int, int> nextPos = pos + line[0] * i;

			if (this->onBoard(nextPos) && this->getValueOnBoard(nextPos) == currentPlayer) {
				numAligned++;
				if (numAligned == 5) {
					endPos = nextPos;
				}
			} else {
				endPos = nextPos - line[0];
				break ;
			}
		}

		for (int i = 1; i < 5; ++i) {
			std::pair<int, int> nextPos = pos + line[1] * i;
			if (this->onBoard(nextPos) && this->getValueOnBoard(nextPos) == currentPlayer) {
				numAligned++;
			} else {
				break ;
			}
		}

		if (numAligned >= 5) {
			if (this->canBreakAlignment(endPos, currentPlayer, currentPlayer == 0 ? 1 : 0, line[1], numAligned)) {
				break ;
			} else {
				int ret = _gameEndingCapture->checkEndGame(*this);
				if (ret > -1) {
					return ret;
				}
			}
		}
	}
	return State::PLAYING;
}

int Gomoku::canBreakAlignment(std::pair<int, int> startPos, int currentPlayer, int otherPlayer, std::pair<int, int>& dir, int numAligned) const {
	std::pair<int, int> nextPos = startPos;
	int unbreakableAligned;

	if (this->canBreakAtPos(nextPos, currentPlayer, otherPlayer)) {
		unbreakableAligned = 0;
	} else {
		unbreakableAligned = 1;
	}

	for (int i = 1; i < numAligned; ++i) {
		nextPos += dir;
		if (this->canBreakAtPos(nextPos, currentPlayer, otherPlayer)) {
			if (numAligned - i < 5) {
				return true;
			}
			unbreakableAligned = 0;
		} else {
			++unbreakableAligned;
			if (unbreakableAligned >= 5) {
				return false;
			}
		}
		if (unbreakableAligned >= 5) {
			return false;
		}
	}
	return true;
}

bool Gomoku::canBreakAtPos(std::pair<int, int> pos, int currentPlayer, int otherPlayer) const {
	for (std::vector<std::pair<int, int>>& line: fourDirections) {
		for (std::pair<int, int>& dir : line) {
			std::pair<int, int> nextPos = pos + dir;

			if (this->onBoard(nextPos) && this->getValueOnBoard(nextPos) == currentPlayer) {
 				nextPos = pos + dir * 2;
				std::pair<int, int> nextPos2 = pos - dir;
				if (this->notOnBoard(nextPos) || this->notOnBoard(nextPos2)) {
					continue;
				}
				if ((this->getValueOnBoard(nextPos) == otherPlayer && this->getValueOnBoard(nextPos2) == -1 && this->canPlace(nextPos2, otherPlayer)) ||
					(this->getValueOnBoard(nextPos) == -1 && this->getValueOnBoard(nextPos2) == otherPlayer && this->canPlace(nextPos, otherPlayer))) {
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

	this->setValueOnBoard(y, x, playerIndex);

	actions.push_back(new ActionUpdateBoard(pos, -1));

	this->updateBoardCallbacks(pos, playerIndex);

	this->remainingStones--;

	for (auto rule : _rules) {
		std::vector<AAction*> ruleActions = rule->triggerEffects(*this, pos);
		actions.insert(actions.end(), ruleActions.begin(), ruleActions.end());
	}

	this->updatePotentialCaptures();

	int state = this->checkWinCondition(pos, playerIndex);
	
	if (state != State::PLAYING) {
		actions.push_back(new ActionSetEndState(this->getEndState()));
		this->setEndState(state);
	}
	return actions;
}

void Gomoku::switchPlayer() {
	if (this->currentPlayer == this->players[0]) {
		this->currentPlayer = this->players[1];
	} else {
		this->currentPlayer = this->players[0];
	}
}

void Gomoku::updatePotentialCaptures() {
	for (std::shared_ptr<Player> player: this->players) {
		player->setPotentialCaptures(0);
	}

	for (int i = 0; i < this->size; i++) {
		this->updatePotentialCapturesByLine(std::make_pair(i, 0), Gomoku::hLine, this->size);
		this->updatePotentialCapturesByLine(std::make_pair(0, i), Gomoku::vLine, this->size);
	}

	for (int i = 0; i <= this->size - 4; i++) {
		this->updatePotentialCapturesByLine(std::make_pair(0, i), Gomoku::dLine1, this->size - i);
		if (i != 0) {
			this->updatePotentialCapturesByLine(std::make_pair(i, 0), Gomoku::dLine1, this->size - i);
		}
	}

	for (int i = this->size - 1; i >= 3; i--) {
		this->updatePotentialCapturesByLine(std::make_pair(0, i), Gomoku::dLine2, i + 1);
	}

	for (int i = 1; i <= this->size - 4; i++) {
		this->updatePotentialCapturesByLine(std::make_pair(i, this->size - 1), Gomoku::dLine2, size - i);
	}
}

void Gomoku::updatePotentialCapturesByLine(std::pair<int, int> startPos, std::pair<int, int> dir, int length) {
	std::pair<int, int> pos;
	int potentialCapturesPatternPositionByPlayer[][2] = {{0, 0}, {0, 0}};
	
	for (int i = 0; i < length; i++) {
		pos = startPos + dir * i;
		for (int p = 0; p < 2; ++p) {
			for (int pat = 0; pat < 2; pat++) {
				if (this->getValueOnBoard(pos) == PotentialCapturePattern[p][pat][ potentialCapturesPatternPositionByPlayer[p][pat] ]) {
					++potentialCapturesPatternPositionByPlayer[p][pat];
					if (potentialCapturesPatternPositionByPlayer[p][pat] == 4) {
						this->players[p]->setPotentialCaptures(this->players[p]->getPotentialCaptures() + 1);
						potentialCapturesPatternPositionByPlayer[p][pat] = 0;
						if (this->getValueOnBoard(pos) == PotentialCapturePattern[p][pat][potentialCapturesPatternPositionByPlayer[p][pat]]) {
							++potentialCapturesPatternPositionByPlayer[p][pat];
						}
					}
				} else {
					potentialCapturesPatternPositionByPlayer[p][pat] = 0;
					if (this->getValueOnBoard(pos) == PotentialCapturePattern[p][pat][potentialCapturesPatternPositionByPlayer[p][pat]]) {
						++potentialCapturesPatternPositionByPlayer[p][pat];
					} 
				}
			}
		}
	}
}

std::vector<std::pair<int, int>> Gomoku::getMoves() {
	std::vector<std::pair<int, int>> moves;
	bool empty = true; 
	int dist = 2;

	for (int i = 0; i < this->size; i++) {
		for (int j = 0; j < this->size; j++) {
			if (this->getValueOnBoard(i, j)  != -1) {
				for (int k = -dist; k <= dist; k++) {
					for (int l = -dist; l <= dist; l++) {
						if (i + k >= 0 && i + k < this->size && j + l >= 0 && j + l < this->size && this->getValueOnBoard(i + k, j + l) == -1) {
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

std::vector<int> Gomoku::hashState() {
	auto hash = _board;
	hash.push_back(this->players[0]->getCaptures());
	hash.push_back(this->players[1]->getCaptures());
	return hash;
}

int Gomoku::evalStreakScore(int currentStreakNum, int currentStreakPotential, bool halfOpen, int player, int emptyCellCount, int* certainVictory) {
	if (currentStreakPotential < 5) {
		return 0;
	}

	if (currentStreakNum >= 5 && emptyCellCount == 0) {
		if (this->getEndState() != -1) {
			std::cout << "SHOULD NOT BE HERE" << std::endl;
			certainVictory[0]++;
			return 0;
		}
	}

	if (player == this->currentPlayer->getIndex()) {
		if (currentStreakNum == 4) {
			certainVictory[1]++;
			return 0;
		}
		if (currentStreakNum == 3 && currentStreakPotential > 5 && !halfOpen && emptyCellCount < 2) {
			certainVictory[3]++;
			return Minmax::CERTAIN_VICTORY / 3;
		}

	} else {
		if (currentStreakNum == 4 && !halfOpen && emptyCellCount == 0) {
			certainVictory[2]++;
			return Minmax::CERTAIN_VICTORY / 2;
		}
	}

	return static_cast<int>(pow((currentStreakNum - emptyCellCount), 3) * (halfOpen ? 1 : 2)) + currentStreakNum;
}

void resetStreak(bool &streaking, bool &frontBlocked, int &currentStreakNum, int& emptyCellCount, int& emptyCellCountPotential) {
	streaking = false;
	frontBlocked = false;
	currentStreakNum = 0;
	emptyCellCountPotential = 0;
	emptyCellCount = currentStreakNum;
}


bool Gomoku::hasEnoughPotential(std::pair<int, int> start, std::pair<int, int> line, int countToCheck, int otherPlayer) {
	auto pos = start;
	while (countToCheck--) {
		pos += line;
		if (this->notOnBoard(pos) || this->getValueOnBoard(pos) == otherPlayer) {
			return false;
		}
	}
	return true;
}

int Gomoku::evalLine(std::pair<int, int> start, std::pair<int, int> line, int player, int length, int* certainVictory) {
	int score = 0;
	bool streaking;
	bool frontBlocked;
	int currentStreakNum;
	int currentStreakPotential = 0;
	int emptyCellCount;
	int emptyCellCountPotential;

	resetStreak(streaking, frontBlocked, currentStreakNum, emptyCellCount, emptyCellCountPotential);

	int otherPlayer = player == 0 ? 1 : 0;

	std::pair<int, int> pos;

	for (int i = 0; i < length; i++) {
		pos = start + line * i;

		if (this->getValueOnBoard(pos) == player) {
			if (!streaking) {
				if (currentStreakNum) {
					if (currentStreakPotential < 5) {
						if (this->hasEnoughPotential(pos, line, 5 - currentStreakPotential, otherPlayer)) {
							score += this->evalStreakScore(currentStreakNum, 100, frontBlocked, player, emptyCellCount, certainVictory);
						}
					} else {
						score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked, player, emptyCellCount, certainVictory);
					}
					resetStreak(streaking, frontBlocked, currentStreakNum, emptyCellCount, emptyCellCountPotential);
					currentStreakNum = 1;
				} else {
					currentStreakNum++;
					currentStreakPotential++;
				}
				frontBlocked = i == 0 || (i > 0 && this->getValueOnBoard(pos - line) == otherPlayer);
				streaking = true;
			} else {
				emptyCellCount += emptyCellCountPotential;
				emptyCellCountPotential = 0;
				streaking = true;
				currentStreakPotential++;
				currentStreakNum++;
			}
		}
		if (this->getValueOnBoard(pos) == -1) {
			currentStreakPotential++;

			if (streaking) {
				emptyCellCountPotential++;
				if (i > 0 && this->getValueOnBoard(pos - line) == -1) {
					streaking = false;
				}
			}
		}

		if (this->getValueOnBoard(pos) == otherPlayer) {
			if (streaking) {
				score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked || (i > 0 && this->getValueOnBoard(pos - line) != -1), player, emptyCellCount, certainVictory);
			} else {
				score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked, player, emptyCellCount, certainVictory);
			}
			resetStreak(streaking, frontBlocked, currentStreakNum, emptyCellCount, emptyCellCountPotential);
			currentStreakPotential = 0;
		}
	}

	score += this->evalStreakScore(currentStreakNum, currentStreakPotential, frontBlocked || this->getValueOnBoard(pos) != -1, player, emptyCellCount, certainVictory);

	return score;
}

int Gomoku::heuristicByPlayer(int player, int* certainVictory) {
	// TODO: possible to iterate only ONCE over board and calc both players at once?

	int captures = this->players[player]->getCaptures();
	if (captures >= 5) {
		std::cout << "LOL MDR SHOULD NOT DU TOUT BE HERE" << std::endl;
		certainVictory[0]++;
		return 0;
	}
	if (player == this->currentPlayer->getIndex()) {
		if (captures == 4 && this->players[player]->getPotentialCaptures() > 0) {
			certainVictory[1]++;
			return 0;
		}
	} else {
		if (captures == 4 && this->players[player]->getPotentialCaptures() > 1) {
			certainVictory[3]++;
			return 0;
		}
	}
	
	int score = 0;

	for (int i = 0; i < this->size; i++) {
		score += this->evalLine(std::make_pair(i, 0), Gomoku::hLine, player, this->size, certainVictory);
		score += this->evalLine(std::make_pair(0, i), Gomoku::vLine, player, this->size, certainVictory);
	}

	for (int i = 0; i <= this->size - this->winStreakLength; i++) {
		score += this->evalLine(std::make_pair(0, i), Gomoku::dLine1, player, this->size - i, certainVictory);
		if (i != 0) {
			score += this->evalLine(std::make_pair(i, 0), Gomoku::dLine1, player, this->size - i, certainVictory);
			if (certainVictory[1] > 0) {
				return 0;
			}
		}
	}

	for (int i = this->size - 1; i >= this->winStreakLength - 1; i--) {
		score += this->evalLine(std::make_pair(0, i), Gomoku::dLine2, player, i + 1, certainVictory);
	}

	for (int i = 1; i <= this->size - this->winStreakLength; i++) {
		score += this->evalLine(std::make_pair(i, this->size - 1), Gomoku::dLine2, player, this->size - i, certainVictory);
	}

	// TODO:

	// if (this->players[0]->getPotentialCaptures() > 0 || this->players[1]->getPotentialCaptures() > 0) {
	// 	std::cout << "pot capteures: " << this->players[0]->getPotentialCaptures() << " and " << this->players[1]->getPotentialCaptures() << std::endl;
	// }
	
	// else {
	// 	if (captures == 4 && potentialCaptures > 0) {
	// 		return Minmax::CERTAIN_VICTORY / 2;
	// 	}
	// }

	// int capturesValue = std::pow(captures + 1, 3);
	// int potentialCapturesValue = capturesValue * potentialCaptures;

	// std::cout << "potentialCaptures: " << potentialCaptures << ", capturesValue: " << capturesValue << ", potentialCapturesValue: " << potentialCapturesValue << std::endl;

	// return capturesValue + potentialCapturesValue;
	return score;// + capturesValue + potentialCapturesValue;
}


int getThresholdValue(int i) {
	switch (i) {
		case 0:
			return Minmax::INF_MAX;
		case 1:
			return 100'000;
		case 2:
			return 50'000;
		case 3:
			return 20'000;
		default:
			return 0;
	}
}


int Gomoku::heuristic(int depth) {
	int certainVictories[][5] = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
	int score0 = this->heuristicByPlayer(0, certainVictories[0]);
	int score1 = this->heuristicByPlayer(1, certainVictories[1]);

	for (int i = 0; i < 5; i++) {
		if (certainVictories[0][i] > 0) {
			score0 += getThresholdValue(i) - (this->minmax->maxDepth - depth);
			break;
		}
		if (certainVictories[1][i] > 0) {
			score1 += getThresholdValue(i) - (this->minmax->maxDepth - depth);
			break;
		}
	}
	return this->heuristicPlayer->getIndex() == 0 ? score0 - score1 : score1 - score0;
}


std::vector<AAction*> Gomoku::doMove(std::pair<int, int>& pos) {
	std::vector<AAction*> actions = this->place(pos.first, pos.second, this->currentPlayer->getIndex());
	this->switchPlayer();
	return actions;
}

bool Gomoku::canPlace(std::pair<int, int> move) const {
	for (auto rule: _rules) {
		if (!rule->canPlace(*this, move)) {
			return false;
		}
	}
	return true;
}

bool Gomoku::canPlace(std::pair<int, int> move, int playerIndex) const {
	for (auto rule: _rules) {
		if (!rule->canPlace(*this, move, playerIndex)) {
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

void Gomoku::printBoard() const {
	for (int j = 0; j < this->size; j++) {
		for (int i = 0; i < this->size; i++) {
			std::cout << std::setw(6) << (this->getValueOnBoard(j, i) == -1 ? "." : std::to_string(this->getValueOnBoard(j, i))) << " ";
		}
		std::cout << std::endl;
	}
}

void Gomoku::printBoard(std::vector<std::vector<int>> board) const {
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

void Gomoku::printBoard(std::vector<std::vector<int>> board, std::pair<int, int> pos) const {
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

void printLineScore(std::vector<int> line, int size, int score, int potentialCaptures) {
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

const std::pair<int, int> Gomoku::hLine = std::make_pair<int, int>(0, 1);
const std::pair<int, int> Gomoku::vLine = std::make_pair<int, int>(1, 0);
const std::pair<int, int> Gomoku::dLine1 = std::make_pair<int, int>(1, 1);
const std::pair<int, int> Gomoku::dLine2 = std::make_pair<int, int>(1, -1);