#include "Common.hpp"

int main(int argc, char *argv[]) {

	args::ArgumentParser parser("This is a test program.", "This goes after the options.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::CompletionFlag completion(parser, {"complete"});
	args::ValueFlag<int> boardSizeArgs(parser, "size", "Size of the board", {'s'});
	args::ValueFlag<int> depthArgs(parser, "depth", "Depth search of MinMax", {'d'});
	args::ValueFlag<int> maxTurnArgs(parser, "maxTurn", "Pause the game after n turns", {'q'});
	args::ValueFlag<int> runTestArgs(parser, "test", "Run unit test set", {'t'});

    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (const args::Completion& e)
    {
        std::cout << e.what();
        return 0;
    }
    catch (const args::Help&)
    {
        std::cout << parser;
        return 0;
    }
    catch (const args::ParseError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

	int boardSize = 19;
	int depth = 4;
	int maxTurn = -1;

	if (boardSizeArgs) {
		boardSize = std::clamp(args::get(boardSizeArgs), 5, 19);
	}
	if (depthArgs) {
		depth = std::clamp(args::get(depthArgs), 2, 10);
	}
	if (maxTurnArgs) {
		maxTurn = std::clamp(args::get(maxTurnArgs), 1, 100'000);
	}
	if (runTestArgs) {
		switch(args::get(runTestArgs)) {
			case 1:
				Gomoku::testEvalLine();
			break;
			case 2:
				Gomoku::testHeuristic();
				break;
			case 3:
				Gomoku::testMinmax();
				break;
		}
		return 0;
	}

	{
		std::shared_ptr<SFMLWindow> window = std::make_shared<SFMLWindow>(1200, 800, "Gomoku");
		std::shared_ptr<Gomoku> gomoku = std::make_shared<Gomoku>(boardSize, Player::AI, Player::AI);
		std::shared_ptr<Minmax> minmax = std::make_shared<Minmax>(*gomoku, depth);

		std::shared_ptr<GUI> gui = std::make_shared<GUI>(gomoku, window);


		gomoku->minmax = minmax;

		gui->setup();

		bool pause = false;
		bool nextStep = false;

		window->loop(std::function<void()>([gomoku, gui, &pause, &nextStep, maxTurn]() mutable {

			if (gomoku->playing && gomoku->currentPlayer->isAI() && (nextStep || !pause)) {
				std::cout << "AI turn" << std::endl;
				auto start = std::chrono::high_resolution_clock::now();
				auto pos = gomoku->minmax->run();
				auto end = std::chrono::high_resolution_clock::now();
				std::cout << "pos: " << pos.first << ", " << pos.second << ", in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
				if (!gui->place(pos.first, pos.second)) {
					return ;
				}
				gomoku->lastMoves[gomoku->currentPlayer->getIndex()] = pos;
				gomoku->place(pos.first, pos.second, gomoku->currentPlayer->getIndex());
				gomoku->switchPlayer();
				gomoku->nextTurn();
				gui->nextTurn();
				nextStep = false;

				if (maxTurn !=-1 && gomoku->getTurn() >= maxTurn) {
					pause = true;
				}
			}
		}), [&pause, &nextStep](sf::Event event) mutable {
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					pause = !pause;
					// std::cout << "pause: " << pause << std::endl;
				}
				if (event.key.code == sf::Keyboard::Period) {
					nextStep = true;
					// std::cout << "pause: " << pause << std::endl;
				}
			}
		});
	}
	return (0);
}