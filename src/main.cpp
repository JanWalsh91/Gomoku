#include <iostream>
#include <memory>

#include "SFMLWindow.hpp"
#include "Player.hpp"

int main() {
	{
		std::cout << "hello" << std::endl;

		std::vector<std::shared_ptr<Player>> players;

		players.push_back(std::make_shared<Player>(0, Player::AI));
		players.push_back(std::make_shared<Player>(1, Player::AI));

		std::shared_ptr<SFMLWindow> window = std::make_shared<SFMLWindow>(1280, 800, "Gomoku");

		window->setupGUI(players);
		window->loop();
	}
	return (0);
}