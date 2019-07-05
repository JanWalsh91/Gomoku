#include <iostream>
#include "Player.hpp"


Player::Player(int index, Type type): index(index), type(type) {
	std::cout << "New Player type: " << type << std::endl;
}