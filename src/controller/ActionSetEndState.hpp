#pragma once

class AAction;

class ActionSetEndState: public AAction {

public:

	ActionSetEndState(int state): AAction(AAction::Type::SET_END_STATE), state(state) {}
	
	virtual ~ActionSetEndState() {}
	
	int state;
};