#include <iostream>
#include "Player.hpp"


Player::Player(int index, Type type): _index(index), _captures(0), _type(type) {
}

bool Player::isAI() const {
	return _type == Player::AI;
}

bool Player::isHuman() const {
	return _type == Player::HUMAN;
}

void Player::changeType(Type type) {
	_type = type;
}

void Player::reset() {
	_captures = 0;
}

void Player::incrementCaptures() {
	++_captures;
}

void Player::decrementCaptures() {
	--_captures;
}

int Player::getCaptures() const {
	return _captures;
}

int Player::getIndex() const {
	return _index;
}

int Player::getPotentialCaptures() const {
	return _potentialCaptures;
}

void Player::setPotentialCaptures(int captures) {
	_potentialCaptures = captures;
}