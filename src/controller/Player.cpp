#include <iostream>
#include "Player.hpp"


Player::Player(int index, Type type): index(index), type(type), captures(0) {
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

void Player::reset() {
	this->captures = 0;
}

void Player::incrementCaptures() {
	++this->captures;
}

void Player::decrementCaptures() {
	--this->captures;
}

int Player::getCaptures() const {
	return this->captures;
}

int Player::getIndex() const {
	return this->index;
}