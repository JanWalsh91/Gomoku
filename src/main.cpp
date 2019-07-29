#include <iostream>
#include <memory>

#include "SFMLWindow.hpp"
#include "Player.hpp"
#include "Gomoku.hpp"
#include "Minmax.hpp"

#include <Windows.h>

int main() {
	{

		std::cout << "hello" << std::endl;

		//std::vector<std::shared_ptr<Player>> players;

		//players.push_back(std::make_shared<Player>(0, Player::AI));
		//players.push_back(std::make_shared<Player>(1, Player::AI));

		std::shared_ptr<SFMLWindow> window = std::make_shared<SFMLWindow>(1200, 800, "Gomoku");
		std::shared_ptr<Gomoku> gomoku = std::make_shared<Gomoku>(5, Player::HUMAN, Player::HUMAN);
		std::shared_ptr<Minmax> minmax = std::make_shared<Minmax>(*gomoku, 2);

		gomoku->minmax = minmax;

		window->setupGUI(gomoku->players, gomoku);
		window->loop(std::function<void()>([gomoku]() {
			//std::cout << "Running: " << gomoku->playing << std::endl;
			//std::cout << "Running: " << gomoku->endState << std::endl;
			//std::cout << "Is Current player AI: " << gomoku->currentPlayer->isAI() << ", idx: " << gomoku->currentPlayer->index << std::endl;


			if (gomoku->playing && gomoku->currentPlayer->isAI()) {
				std::cout << "AI turn" << std::endl;
				auto pos = gomoku->minmax->run();
				std::cout << "pos: " << pos.first << ", " << pos.second << std::endl;
				gomoku->place(pos.first, pos.second, gomoku->currentPlayer->index);
				gomoku->switchPlayer();

				// check for win conditions
				// TODO: separate GUI and window, create reference and capture gui here
			}

			// If is ai turn, run the minmax algorithm
			// Wait for the end of the result
			// play the result

			//Sleep(2000);
		}));
	}
	return (0);
}