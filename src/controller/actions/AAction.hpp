#pragma once

class Gomoku;

class AAction {

public:

	virtual ~AAction() = 0;
	virtual void execute(Gomoku& gomoku) = 0;

private:

};