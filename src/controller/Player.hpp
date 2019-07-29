#pragma once

class Player {

public:

	enum Type {
		AI,
		HUMAN
	};

	Player(int index, Type type);

	bool isAI() const;
	bool isHuman() const;
	void changeType(Type t);

	int index;
	Type type;
};