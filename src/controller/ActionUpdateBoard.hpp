#pragma once

class AAction;

class ActionUpdateBoard: public AAction {
public:

	ActionUpdateBoard(std::pair<int, int> pos, int value): AAction(AAction::Type::UPDATE_BOARD), pos(pos), value(value) {

		std::cout << "Instanceiateiotje ActionUpdateBoard " << pos.first << ", " << pos.second << " with value " << value << std::endl;

	}

	virtual ~ActionUpdateBoard() {}

	std::pair<int, int> pos;
	int value;

};