#include "Common.hpp"

int main(int argc, char *argv[]) {
	std::srand(std::time(nullptr));

	args::ArgumentParser parser("Gomoku options", "Thanks for playing.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::CompletionFlag completion(parser, {"complete"});
	args::ValueFlag<int> boardSizeArgs(parser, "size", "Size of the board", {'s'});
	args::ValueFlag<int> depthArgs(parser, "depth", "Depth search of MinMax", {'d'});
	args::ValueFlag<int> maxTurnArgs(parser, "maxTurn", "Pause the game after n turns", {'q'});
	args::ValueFlag<int> runTestArgs(parser, "test", "Run unit test set", {'t'});
	args::ValueFlag<int> testIndexArgs(parser, "testIndex", "Test index", {'i'});
	args::ValueFlag<int> startConfigurationArgs(parser, "startConfiguration", "Start Configuration index", {'c'});
	args::ValueFlag<std::string> playersType(parser, "playersType", "Player Type, AI or Human (a / h)", {'p'});
	args::Flag autoStartArgs(parser, "autoStart", "Auto Start", {'a'});

    try {
        parser.ParseCLI(argc, argv);
    }
    catch (const args::Completion& e) {
        std::cout << e.what();
        return 0;
    }
	catch (const args::Help&) {
        std::cout << parser;
        return 0;
    }
	catch (const args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

	int boardSize = 19;
	int depth = 3;
	int maxTurn = -1;
	bool autoStart = false;
	int startConfiguration = -1;

	if (boardSizeArgs) {
		boardSize = std::clamp(args::get(boardSizeArgs), 5, 19);
	}
	if (depthArgs) {
		depth = std::clamp(args::get(depthArgs), 2, 4);
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
	if (autoStartArgs) {
		autoStart = args::get(autoStartArgs);
	}
	if (startConfigurationArgs) {
		startConfiguration = args::get(startConfigurationArgs);
		if (startConfiguration >= 0) {
			boardSize = 19;
		}
	}

	std::pair<Player::Type, Player::Type> playerTypes = { Player::HUMAN, Player::AI };
	if (playersType) {
		std::string playersTypeString = args::get(playersType);

		if (playersTypeString.size() == 2) {
			if (playersTypeString[0] == 'a') {
				playerTypes.first = Player::AI;
			} else if (playersTypeString[0] == 'h') {
				playerTypes.first = Player::HUMAN;
			}
			if (playersTypeString[1] == 'a') {
				playerTypes.second = Player::AI;
			} else if (playersTypeString[1] == 'h') {
				playerTypes.second = Player::HUMAN;
			}
		}
	}
	

	std::shared_ptr<SFMLWindow> window = std::make_shared<SFMLWindow>(1200, 800, "Gomoku");
	std::shared_ptr<Gomoku> gomoku = std::make_shared<Gomoku>(boardSize, playerTypes.first, playerTypes.second);
	std::shared_ptr<Minmax> minmax = std::make_shared<Minmax>(*gomoku, depth);
	std::shared_ptr<GUI> gui = std::make_shared<GUI>(gomoku, window);


	gomoku->minmax = minmax;

	gui->setup();

	bool pause = false;
	bool nextStep = false;

	std::future<std::pair<int, int>> future;

	gomoku->onUpdateBoard([&gui, gomoku](std::pair<int, int> pos, int value) {
		gui->updateBoard(pos, value);
	});

	gomoku->onCapture([&gui, gomoku](int playerIndex, int value) {
		gui->updateCaptures(playerIndex, value);
	});

	if (startConfiguration >= 0) {
		gomoku->setStartConfigurationIndex(startConfiguration);
		gomoku->setUpStartConfiguration(startConfiguration);
	}

	if (autoStart) {
		gomoku->playing = true;
	}

	window->loop(std::function<void()>([gomoku, gui, &pause, &nextStep, maxTurn, &future]() mutable {
		
		if (future.valid() && future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
			auto pos = future.get();
			if (gomoku->shouldReset()) {
				gomoku->clearResetRequest();
				gomoku->reset();
				return ;
			}
			std::cout << "pos: " << pos << std::endl;
			if (pos.first < 0 || pos.second < 0) {
				return;
			}
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
			future = std::async(std::launch::async, &Minmax::run, gomoku->minmax);
		}
	}), [&pause, &nextStep](sf::Event event) mutable {
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Space) {
				pause = !pause;
			}
			if (event.key.code == sf::Keyboard::Period) {
				nextStep = true;
			}
		}
	});
	return (0);
}
