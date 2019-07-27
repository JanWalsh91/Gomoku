#pragma once

#include "Gomoku.hpp"
#include "AAction.hpp"
#include <string>

class ARule {

public:
	std::string name;

	ARule(std::string name): name(name) {}
	virtual bool canPlace(Gomoku& gomoku, std::pair<int, int>& pos) { return true; }
	virtual std::vector<AAction*> triggerEffects(Gomoku& gomoku, std::pair<int, int>& pos) { return std::vector<AAction*>(); }
	virtual bool isWinningCondition(Gomoku& gomoku) { return false; }

};
