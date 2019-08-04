#include "Common.hpp"

int main(int argc, char *argv[]) {
	std::srand(std::time(nullptr));

	args::ArgumentParser parser("This is a test program.", "This goes after the options.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::CompletionFlag completion(parser, {"complete"});
	args::ValueFlag<int> boardSizeArgs(parser, "size", "Size of the board", {'s'});
	args::ValueFlag<int> depthArgs(parser, "depth", "Depth search of MinMax", {'d'});
	args::ValueFlag<int> maxTurnArgs(parser, "maxTurn", "Pause the game after n turns", {'q'});
	args::ValueFlag<int> runTestArgs(parser, "test", "Run unit test set", {'t'});
	args::ValueFlag<int> testIndexArgs(parser, "testIndex", "Test index", {'i'});

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
		int runTest = std::clamp(args::get(runTestArgs), 1, 3);
		
		std::vector<std::function<void(void)>> allTestsFn = {
			static_cast<void(*)(void)>(&Tests::runEvalLine),
			static_cast<void(*)(void)>(&Tests::runHeuristic),
			static_cast<void(*)(void)>(&Tests::runMinmax),
		};

		std::vector<std::function<void(int)>> testIndexFn = {
			static_cast<void(*)(int)>(&Tests::runEvalLine),
			static_cast<void(*)(int)>(&Tests::runHeuristic),
			static_cast<void(*)(int)>(&Tests::runMinmax),
		};

		Tests::init();
		if (testIndexArgs) {
			int testIndex = args::get(testIndexArgs);
			testIndexFn[runTest - 1](testIndex);
		} else {
			allTestsFn[runTest - 1]();
		}
		return 0;
	}

	{
		std::shared_ptr<SFMLWindow> window = std::make_shared<SFMLWindow>(1200, 800, "Gomoku");
		std::shared_ptr<Gomoku> gomoku = std::make_shared<Gomoku>(boardSize, Player::HUMAN, Player::HUMAN);
		std::shared_ptr<Minmax> minmax = std::make_shared<Minmax>(*gomoku, depth);

		std::shared_ptr<GUI> gui = std::make_shared<GUI>(gomoku, window);


		gomoku->minmax = minmax;

		gui->setup();

		bool pause = false;
		bool nextStep = false;

		std::future<std::pair<int, int>> future;

		gomoku->onUpdateBoard([gui, gomoku](std::pair<int, int> pos, int value) {
			gui->updateBoard(pos, value);
		});

		gomoku->onCapture([gui, gomoku](int playerIndex, int value) {
			gui->updateCaptures(playerIndex, value);
		});

		auto start = std::chrono::high_resolution_clock::now();
		auto end = std::chrono::high_resolution_clock::now();

		window->loop(std::function<void()>([gomoku, gui, &pause, &nextStep, maxTurn, &future, &start, &end]() mutable {

			if (future.valid() && future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
				if (gomoku->hasBeenReset()) {
					std::cout << "Has been reset in between, whoops!" << std::endl;
					gomoku->clearReset();
					return ;
				}
				auto pos = future.get();
				end = std::chrono::high_resolution_clock::now();
				std::cout << "pos: " << pos.first << ", " << pos.second << ", in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
				gomoku->lastMoves[gomoku->currentPlayer->getIndex()] = pos;
				gomoku->place(pos.first, pos.second, gomoku->currentPlayer->getIndex());
				gomoku->switchPlayer();
				gui->nextTurn();
				nextStep = false;

				if (maxTurn !=-1 && gomoku->getTurn() >= maxTurn) {
					pause = true;
				}
			}
			if (gomoku->playing && gomoku->currentPlayer->isAI() && (nextStep || !pause) && !gomoku->minmax->isRunning()) {
				std::cout << "AI turn" << std::endl;
				start = std::chrono::high_resolution_clock::now();

				future = std::async(std::launch::async, &Minmax::run, gomoku->minmax);
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