#pragma once

#include "Common.hpp"

class Gomoku;

class Captures: public ARule {
public:
	Captures();
	virtual std::vector<AAction*> triggerEffects(Gomoku& gomoku, std::pair<int, int>& pos);
	virtual int checkEndGame(const Gomoku& gomoku);
};
