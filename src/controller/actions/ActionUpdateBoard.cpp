#include "ActionUpdateBoard.hpp"

ActionUpdateBoard::ActionUpdateBoard(std::pair<int, int> pos, int value): _pos(pos), _value(value) {}
ActionUpdateBoard::~ActionUpdateBoard() {}

void ActionUpdateBoard::execute(Gomoku& gomoku) {
	gomoku.board[_pos.first][_pos.second] = _value;
	gomoku.remainingStones += _value >= 0 ? -1 : 1;
	gomoku.updateBoardCallbacks(_pos, _value);
}