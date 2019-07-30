#pragma once

#include "Common.hpp"

class ARule {

public:
	std::string name;

	ARule(std::string name): name(name) {}
	virtual bool canPlace(Gomoku& gomoku, std::pair<int, int>& pos) {
		(void)gomoku;
		(void)pos;
		return true;
	}
	virtual std::vector<AAction*> triggerEffects(Gomoku& gomoku, std::pair<int, int>& pos) {
		(void)gomoku;
		(void)pos;
		return std::vector<AAction*>();
	}
	virtual bool isWinningCondition(Gomoku& gomoku) {
		(void)gomoku;
		return false;
	}

};
