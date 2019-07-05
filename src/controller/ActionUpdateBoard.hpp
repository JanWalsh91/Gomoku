#pragma once

class AAction;

class ActionUpdateBoard: public AAction {
public:

	ActionUpdateBoard(std::pair<int, int> pos, int value): AAction(AAction::Type::UPDATE_BOARD), pos(pos), value(value) {}

	virtual ~ActionUpdateBoard() {}

	std::pair<int, int> pos;
	int value;

};