#pragma once

#include "Common.hpp"

class Gomoku;

class GameEndingCapture: public ARule {
public:
	GameEndingCapture();
	virtual int checkEndGame(const Gomoku& gomoku);
};
