#pragma once

#include "Common.hpp"

class Gomoku;

class ARule {

public:
	std::string name;

	ARule(std::string name): name(name) {}
	virtual bool canPlace(const Gomoku& gomoku, std::pair<int, int>& pos) {
		(void)gomoku;
		(void)pos;
		return true;
	}
	virtual std::vector<AAction*> triggerEffects(const Gomoku& gomoku, std::pair<int, int>& pos) {
		(void)gomoku;
		(void)pos;
		return std::vector<AAction*>();
	}
	virtual int checkEndGame(const Gomoku& gomoku) {
		(void)gomoku;
		return -1;
	}

};
