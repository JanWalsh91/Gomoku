#pragma once

#include "Common.hpp"

class Gomoku;
class AAction;

class ARule {

public:
	std::string name;

	ARule(std::string name): name(name) {}
	virtual bool canPlace(const Gomoku& gomoku, std::pair<int, int>& pos) {
		(void)gomoku;
		(void)pos;
		return true;
	}
	virtual bool canPlace(const Gomoku& gomoku, std::pair<int, int>& pos, int playerIndex) {
		(void)gomoku;
		(void)pos;
		(void)playerIndex;
		return true;
	}
	virtual std::vector<AAction*> triggerEffects(Gomoku& gomoku, std::pair<int, int>& pos) {
		(void)gomoku;
		(void)pos;
		return std::vector<AAction*>();
	}
	virtual int checkEndGame(const Gomoku& gomoku) {
		(void)gomoku;
		return -1;
	}

};
