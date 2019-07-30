#pragma once

#include "AAction.hpp"

class ActionIncrementCapture: AAction {

public:

	ActionIncrementCapture(int playerIndex): AAction(AAction::Type::INCREMENT_CAPTURE), playerIndex(playerIndex) {}

	int playerIndex;
};