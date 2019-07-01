#pragma once

class AAction;

class ActionDecrementCapture: public AAction {

public:

	ActionDecrementCapture(int playerIndex): AAction(AAction::Type::DECREMENT_CAPTURE), playerIndex(playerIndex) {}

	int playerIndex;
};