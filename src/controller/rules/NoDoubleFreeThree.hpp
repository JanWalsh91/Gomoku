#pragma once

#include "Common.hpp"

class Gomoku;

class NoDoubleFreeThree: public ARule {
public:
	NoDoubleFreeThree();
	virtual bool canPlace(const Gomoku& gomoku, std::pair<int, int>& move);
};
