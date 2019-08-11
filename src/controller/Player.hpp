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
	void reset();
	void incrementCaptures();
	void decrementCaptures();
	int getCaptures() const;
	int getIndex() const;
	int getPotentialCaptures() const;
	void setPotentialCaptures(int captures);
	
private:

	int _index;
	int _captures;
	int _potentialCaptures;
	

	Type _type;
};