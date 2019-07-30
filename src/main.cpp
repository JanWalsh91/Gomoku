#include <iostream>
#include <memory>

#include "SFMLWindow.hpp"
#include "Player.hpp"
#include "Gomoku.hpp"
#include "Minmax.hpp"
#include "GUI.hpp"

// #include <Windows.h>

int main() {
	{
		std::shared_ptr<SFMLWindow> window = std::make_shared<SFMLWindow>(1200, 800, "Gomoku");
		std::shared_ptr<Gomoku> gomoku = std::make_shared<Gomoku>(5, Player::HUMAN, Player::HUMAN);
		std::shared_ptr<Minmax> minmax = std::make_shared<Minmax>(*gomoku, 2);

		std::shared_ptr<GUI> gui = std::make_shared<GUI>(gomoku, window);


		gomoku->minmax = minmax;

		gui->setup();

		window->loop(std::function<void()>([gomoku, gui]() {

			if (gomoku->playing && gomoku->currentPlayer->isAI()) {
				std::cout << "AI turn" << std::endl;
				auto pos = gomoku->minmax->run();
				std::cout << "pos: " << pos.first << ", " << pos.second << std::endl;
				gomoku->place(pos.first, pos.second, gomoku->currentPlayer->index);
				gui->place(pos.first, pos.second);
				gomoku->switchPlayer();
				gui->nextTurn();

			}
		}));
	}
	return (0);
}