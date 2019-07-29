#include <iostream>
#include "Player.hpp"


Player::Player(int index, Type type): index(index), type(type) {
	std::cout << "New Player type: " << type << std::endl;
}

bool Player::isAI() const {
	return this->type == Player::AI;
}

bool Player::isHuman() const {
	return this->type == Player::HUMAN;
}

void Player::changeType(Type type) {
	this->type = type;
}