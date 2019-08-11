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
	if (autoStartArgs) {
		autoStart = args::get(autoStartArgs);
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

		std::future<std::pair<int, int>> future;

		gomoku->onUpdateBoard([gui, gomoku](std::pair<int, int> pos, int value) {
			gui->updateBoard(pos, value);
		});

		gomoku->onCapture([gui, gomoku](int playerIndex, int value) {
			gui->updateCaptures(playerIndex, value);
		});

		auto start = std::chrono::high_resolution_clock::now();
		auto end = std::chrono::high_resolution_clock::now();

		// ============ Tests for 5 aligned ================

		#if 0
		{
			// x x x x
			gomoku->place(2, 1, 0);
			gomoku->place(2, 2, 0);
			gomoku->place(2, 3, 0);
			gomoku->place(2, 4, 0);
			#if 1
			// x! x x x
			{
				gomoku->place(1, 1, 1);
				gomoku->place(3, 1, 0);
			}
			#endif
			#if 0
			// x x !x x
			{
				gomoku->place(1, 3, 1);
				gomoku->place(3, 3, 0);
			}
			#endif
		}
		#endif
		#if 0
		{
			// x x x x . x x x
			gomoku->place(2, 1, 0);
			gomoku->place(2, 2, 0);
			gomoku->place(2, 3, 0);
			gomoku->place(2, 4, 0);
			gomoku->place(2, 6, 0);
			gomoku->place(2, 7, 0);
			gomoku->place(2, 8, 0);
			#if 0
			// x x x x . !x x x
			{
				gomoku->place(1, 6, 1);
				gomoku->place(3, 6, 0);	
			}
			#endif
			#if 0
			// x x x !x . x x x
			{
				gomoku->place(1, 4, 1);
				gomoku->place(3, 4, 0);	
			}
			#endif
			#if 0
			// x x x !x . x x x x
			{
				gomoku->place(2, 9, 0);
				gomoku->place(1, 4, 1);
				gomoku->place(3, 4, 0);
			}
			#endif
			#if 0
			// x x !x x . x x x x x x
			{
				gomoku->place(2, 9, 0);
				gomoku->place(2, 10, 0);
				gomoku->place(2, 11, 0);
				gomoku->place(1, 3, 1);
				gomoku->place(3, 3, 0);
			}
			#endif
		}
		#endif
		#if 0
		// currentPlayer cannot place and break
		// x x !x x . x x x
		{
			gomoku->place(2, 1, 0);
			gomoku->place(2, 2, 0);
			gomoku->place(2, 3, 0);
			gomoku->place(2, 4, 0);
			
			gomoku->place(1, 4, 1);
			gomoku->place(3, 4, 0);
			gomoku->place(4, 2, 1);
			gomoku->place(4, 3, 1);
			gomoku->place(2, 6, 1);
			gomoku->place(3, 5, 1);

		}
		#endif
		#if 0
		// x x !x x . x x x            HORIZONTAL
		{
			gomoku->place(2, 1, 0);
			gomoku->place(2, 2, 0);
			gomoku->place(2, 3, 0);
			gomoku->place(2, 5, 0);
			
			#if 0
			gomoku->place(1, 3, 1);
			gomoku->place(3, 5, 0);
			#endif
			#if 0
			gomoku->place(1, 3, 0);
			gomoku->place(3, 5, 1);
			#endif

			#if 0
			gomoku->place(1, 5, 1);
			gomoku->place(3, 3, 0);
			#endif
			#if 0
			gomoku->place(1, 5, 0);
			gomoku->place(3, 3, 1);
			#endif

			#if 0
			gomoku->place(1, 4, 1);
			gomoku->place(3, 4, 0);
			#endif
			#if 0
			gomoku->place(1, 4, 0);
			gomoku->place(3, 4, 1);
			#endif
		}
		#endif
		
		#if 0
		//           VeRTICAL
		gomoku->place(1, 2, 0);
		gomoku->place(2, 2, 0);
		gomoku->place(3, 2, 0);
		gomoku->place(5, 2, 0);
		
		#if 0
		gomoku->place(4, 3, 0);
		gomoku->place(6, 1, 1);
		#endif
		#if 0
		gomoku->place(4, 3, 1);
		gomoku->place(6, 1, 0);
		#endif

		#if 0
		gomoku->place(5, 3, 0);
		gomoku->place(5, 1, 1);
		#endif
		#if 0
		gomoku->place(5, 3, 1);
		gomoku->place(5, 1, 0);
		#endif

		#if 0
		gomoku->place(4, 1, 1);
		gomoku->place(6, 3, 0);
		#endif
		#if 0
		gomoku->place(4, 1, 0);
		gomoku->place(6, 3, 1);
		#endif
		#endif

		#if 0
		// DIAGONAL 1
		gomoku->place(1, 1, 0);
		gomoku->place(2, 2, 0);
		gomoku->place(4, 4, 0);
		gomoku->place(5, 5, 0);

		#if 0
		gomoku->place(2, 3, 0);
		gomoku->place(4, 3, 1);
		#endif
		#if 0
		gomoku->place(2, 3, 1);
		gomoku->place(4, 3, 0);
		#endif
		
		#if 0
		gomoku->place(2, 4, 1);
		gomoku->place(4, 2, 0);
		#endif
		#if 0
		gomoku->place(2, 4, 0);
		gomoku->place(4, 2, 1);
		#endif

		#if 0
		gomoku->place(3, 4, 0);
		gomoku->place(3, 2, 1);
		#endif
		#if 1
		gomoku->place(3, 4, 0);
		gomoku->place(3, 2, 1);
		#endif

		#endif

		#if 0
		// DIAGONAL 2
		gomoku->place(1, 5, 0);
		gomoku->place(2, 4, 0);
		gomoku->place(4, 2, 0);
		gomoku->place(5, 1, 0);

		#if 0
		gomoku->place(2, 3, 0);
		gomoku->place(4, 3, 1);
		#endif
		#if 0
		gomoku->place(2, 3, 1);
		gomoku->place(4, 3, 0);
		#endif
		
		#if 0
		gomoku->place(2, 2, 1);
		gomoku->place(4, 4, 0);
		#else
		gomoku->place(2, 2, 0);
		gomoku->place(4, 4, 1);
		#endif

		#if 0
		gomoku->place(3, 4, 0);
		gomoku->place(3, 2, 1);
		#endif
		#if 0
		gomoku->place(3, 4, 0);
		gomoku->place(3, 2, 1);
		#endif


		#endif

		#if 0
		gomoku->place(7, 8, 0);
		gomoku->place(9, 8, 1);
		gomoku->place(8, 7, 0);
		gomoku->place(8, 10, 1);
		gomoku->place(6, 8, 1);
		gomoku->place(7, 8, 0);
		gomoku->place(8, 9, 1);
		gomoku->place(6, 9, 0);
		gomoku->place(5, 10, 1);
		gomoku->place(6, 8, 1);
		gomoku->place(7, 5, 0);
		gomoku->place(3, 8, 1);
		gomoku->place(10, 5, 0);
		gomoku->place(9, 6, 1);
		gomoku->place(8, 5, 0);
		gomoku->place(9, 5, 1);
		gomoku->place(4, 9, 1);
		gomoku->place(9, 7, 0);
		gomoku->place(9, 6, 1);
		gomoku->place(9, 5, 0);
		gomoku->place(9, 3, 1);
		gomoku->place(9, 4, 0);
		gomoku->place(9, 5, 0);
		gomoku->place(6, 11, 1);
		gomoku->place(7, 7, 1);

		// gomoku->place(6, 5, 0);
		// gomoku->place(2, 7, 1);

		gomoku->players[0]->incrementCaptures();
		gomoku->players[0]->incrementCaptures();
		gomoku->players[0]->incrementCaptures();

		gomoku->players[1]->incrementCaptures();
		gomoku->players[1]->incrementCaptures();
		gomoku->players[1]->incrementCaptures();
		gomoku->players[1]->incrementCaptures();
		#endif

		if (autoStart) {
			std::cout << "Auto Start: " << autoStart << std::endl;
			gomoku->playing = true;
		}
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
				}
				if (event.key.code == sf::Keyboard::Period) {
					nextStep = true;
				}
			}
		});
	}
	return (0);
}