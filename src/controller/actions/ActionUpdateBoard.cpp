#include "ActionUpdateBoard.hpp"

ActionUpdateBoard::ActionUpdateBoard(std::pair<int, int> pos, int value): _pos(pos), _value(value) {}
ActionUpdateBoard::~ActionUpdateBoard() {}

void ActionUpdateBoard::execute(Gomoku& gomoku) {
	gomoku.setValueOnBoard(_pos, _value);
	gomoku.setRemainingStones(gomoku.getRemainingStones() + (_value >= 0 ? -1 : 1));
	gomoku.updateBoardCallbacks(_pos, _value);
}