#include "Common.hpp"

// #include <ctime>
// #include <ratio>
// #include <chrono>

void Tests::init() {
	Tests::_gomoku->minmax = Tests::_minmax;
}

// === EVAL LINE === //

void Tests::runEvalLine(int index) {
	std::cout << "runEvalLine " << index << std::endl;
	if (index == -1) {
		Tests::runEvalLine(static_cast<int>(Tests::_evalLineTestCases.size() - 1));
		return ;
	}
	if (0 < index && index < (int)_evalLineTestCases.size()) {
		Tests::_evalLine(Tests::_evalLineTestCases[index]);
	} else {
		std::cout << "Test case " << index << " does not exist"  << std::endl;
		return ;
	}

}

void Tests::runEvalLine() {
	for (size_t i = 0; i < Tests::_evalLineTestCases.size(); i++) {
		Tests::runEvalLine(i);
	}
}

void Tests::_evalLine(Tests::EvalLineParams& param) {
	std::cout << " === Unit Test: Eval Line " << std::endl;

	Tests::_gomoku->size = param.line.size();
	Tests::_gomoku->reset();
	for (int i = 0; i < _gomoku->size; i++) {
		Tests::_gomoku->setValueOnBoard(0, i, param.line[i]);
	}

	int certainVictories[5] = {0, 0, 0, 0, 0};

	int score = Tests::_gomoku->evalLine(std::make_pair(0, 0), std::make_pair(0, 1), 0, Tests::_gomoku->size, certainVictories);

	std::cout << "certainVictories: ";
	for (int i = 0; i < 5; i++) {
		if (i == 4) {
			std::cout << certainVictories[i];
		} else {
			std::cout << certainVictories[i] << ", ";
		}
	}
	std::cout << std::endl;

	for (int i = 0; i < Tests::_gomoku->size; i ++) {
		if (i == Tests::_gomoku->size - 1) {
			std::cout << static_cast<int>(Tests::_gomoku->getValueOnBoard(0, i));
		} else {
			std::cout << static_cast<int>(Tests::_gomoku->getValueOnBoard(0, i)) << ", ";
		}
	}
	std::cout << "\t => score: " << score << std::endl << std::endl;
}

// === HEURISTIC === //

void Tests::runHeuristic(int index) {
	std::cout << "runHeuristic " << index << std::endl;
	if (index == -1) {
		Tests::runHeuristic(static_cast<int>(Tests::_heuristicTestCases.size() - 1));
		return ;
	} 
	if (0 < index && index < (int)_heuristicTestCases.size()) {
		Tests::_heuristic(Tests::_heuristicTestCases[index]);
	} else {
		std::cout << "Test case " << index << " does not exist"  << std::endl;
		return ;
	}
}

void Tests::runHeuristic() {
	for (size_t i = 0; i < Tests::_heuristicTestCases.size(); i++) {
		Tests::runHeuristic(i);
	}
}

void Tests::_heuristic(Tests::HeuristicParams& param) {
	std::cout << " === Unit Test: Heuristic " << std::endl;

	Tests::_gomoku->size = param.board.size();
	Tests::_gomoku->reset();
	for (int y = 0; y < _gomoku->size; y++) {
		for (int i = 0; i < _gomoku->size; i++) {
			Tests::_gomoku->setValueOnBoard(y, i, param.board[y][i]);
		}
	}


	Tests::_gomoku->currentPlayer = Tests::_gomoku->heuristicPlayer = Tests::_gomoku->players[param.currentPlayer];

	auto heuristicValues = std::vector<std::vector<int>>(Tests::_gomoku->size, std::vector<int>(Tests::_gomoku->size, 0));
	
	std::vector<std::pair<int, int>> moves;

	if (param.moves.size()) {
		moves = param.moves;
	} else {
		moves = Tests::_gomoku->getMoves();
	}

	int bestValue = Minmax::LOSS;
	std::pair<int, int> bestMove;

	for (int i = 0; i < param.capturesPerPlayer[0]; i++) {
		Tests::_gomoku->players[0]->incrementCaptures();
	}
	for (int i = 0; i < param.capturesPerPlayer[1]; i++) {
		Tests::_gomoku->players[1]->incrementCaptures();
	}

	for (auto &move: moves) {
		auto undoMoves = Tests::_gomoku->doMove(move);

		int ret = Tests::_gomoku->heuristic(0);
		
		heuristicValues[move.first][move.second] = ret;

		if (ret > bestValue) {
			bestValue = ret;
			bestMove = move;
		}

		Tests::_gomoku->undoMove(undoMoves);
	}
	Tests::_gomoku->printBoard();
	std::cout << "==========" << std::endl;
	Tests::_gomoku->printBoard(heuristicValues, bestMove);
	std::cout << "=======\n";
}

// === MINMAX === //

void Tests::runMinmax(int index) {
	if (index == -1) {
		Tests::runMinmax(static_cast<int>(Tests::_minmaxTestCases.size() - 1));
		return ;
	} 
	if (0 <= index && index < (int)_minmaxTestCases.size()) {
		Tests::_minmaxUnitTest(Tests::_minmaxTestCases[index]);
	} else {
		std::cout << "Test case " << index << " does not exist"  << std::endl;
		return ;
	}
}

void Tests::runMinmax() {
	for (size_t i = 0; i < Tests::_minmaxTestCases.size(); i++) {
		Tests::runMinmax(i);
	}
}

void Tests::_minmaxUnitTest(Tests::MinmaxParams& param) {
	std::cout << " === Unit Test: Minmax. Current Player: " << Tests::_gomoku->players[param.currentPlayer]->getIndex() << std::endl;

	Tests::_gomoku->size = param.board.size();
	Tests::_gomoku->reset();
	for (int y = 0; y < _gomoku->size; y++) {
		for (int i = 0; i < _gomoku->size; i++) {
			Tests::_gomoku->setValueOnBoard(y, i, param.board[y][i]);
		}
	}

	Tests::_gomoku->currentPlayer = Tests::_gomoku->heuristicPlayer = Tests::_gomoku->players[param.currentPlayer];

	auto heuristicValues = std::vector<std::vector<int>>(Tests::_gomoku->size, std::vector<int>(Tests::_gomoku->size, 0));
	
	std::vector<std::pair<int, int>> moves;

	std::vector<int> depths;

	for (int i = 0; i < param.capturesPerPlayer[0]; i++) {
		Tests::_gomoku->players[0]->incrementCaptures();
	}
	for (int i = 0; i < param.capturesPerPlayer[1]; i++) {
		Tests::_gomoku->players[1]->incrementCaptures();
	}

	if (param.depths.size() > 0) {
		depths = param.depths;
	} else {
		depths = { Tests::_DefaultDepth };
	}

	for (int& depth : depths) {
		Tests::_minmax->setMaxDepth(depth);
		Tests::_gomoku->printBoard();
		std::cout << "===== DEPTH " << depth << " =====" << std::endl;
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		Tests::_minmax->run();
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::cout << "time: " << std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count() << std::endl;
	}
}



int Tests::_DefaultSize = 19;
int Tests::_DefaultDepth = 4;
std::shared_ptr<Gomoku> Tests::_gomoku = std::make_shared<Gomoku>(Tests::_DefaultSize, Player::Type::AI, Player::Type::AI);
std::shared_ptr<Minmax> Tests::_minmax = std::make_shared<Minmax>(*Tests::_gomoku, Tests::_DefaultDepth);

std::vector<Tests::EvalLineParams> Tests::_evalLineTestCases = {
	{
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	},
	{
		{ -1,  0, -1, -1, -1, -1, -1, -1, -1, -1 },
	},
	{
		{ -1,  0,  0, -1, -1, -1, -1, -1, -1, -1 },
	},
	{
		{ -1,  0,  0,  0, -1, -1, -1, -1, -1, -1 },
	},
	{
		{ -1,  0,  0,  0,  0, -1, -1, -1, -1, -1 },
	},
	{
		{ -1,  0,  0,  0,  0,  0, -1, -1, -1, -1 },
	},
	{
		{ -1,  0,  0,  0,  0,  0,  0, -1, -1, -1 },
	},
	{
		{-1,  0,  0, -1, -1, -1, -1, -1, -1, -1 },
	},
	{
		{  1,  0,  0, -1, -1, -1, -1, -1, -1, -1 },
	},
	{
		{  0,  0, -1, -1, -1, -1, -1, -1, -1, -1 },
	},
	{
		{-1,  0,  0,  0, -1, -1, -1, -1, -1, -1 },
	},
	{
		{  1,  0,  0,  0, -1, -1, -1, -1, -1, -1 },
	},
	{
		{  0,  0,  0, -1, -1, -1, -1, -1, -1, -1 },
	},
	{
		{ -1,  0,  0, -1, -1, -1, -1, -1, -1, -1 },
	},
	{
		{ -1,  0, -1,  0, -1, -1, -1, -1, -1, -1 },
	},
	{
		{ -1,  0,  0,  0, -1, -1, -1, -1, -1, -1 },
	},
	{
		{ -1,  0,  0, -1,  0, -1, -1, -1, -1, -1 },
	},
	{
		{ -1,  0,  0,  0,  0, -1, -1, -1, -1, -1 },
	},
	{
		{ -1,  0,  0, -1,  0,  0, -1, -1, -1, -1 },
	},
	{
		{ -1,  0, -1,  0,  0,  0, -1, -1, -1, -1 },
	},
	{
		{ -1,  0,  0,  0,  0,  0, -1, -1, -1, -1 },
	},
	{
		{ -1,  0,  0, -1,  0,  0,  0, -1, -1, -1 },
	},
	{
		{ -1,  0, -1, -1,  0,  0,  0,  0, -1, -1 },
	},
	{
		{ -1,  1,  0,  0, -1, -1, -1 },
	},
	{
		{ -1, -1,  0,  0,  1, -1, -1 },
	},
	{
		{  1,  0,  0, -1,  0,  0,  1 },
	},
};

std::vector<Tests::HeuristicParams> Tests::_heuristicTestCases = {
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1,  0, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1,  0, -1, -1, -1, -1, -1 },
			{ -1, -1, -1,  0, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		},
		0,
		{ 4, 1 },
		{}
	},
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1,  0, -1, -1, -1,  0, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1,  0, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1,  0, -1, -1, -1,  0, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
		},
		0,
		{0, 0},
		{}
	},
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1,  0, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
		},
		0,
		{0, 0},
		{}
	},
	{
		{
			{  0, -1, -1, -1, -1, -1,  0 },
			{ -1,  0, -1, -1, -1,  0, -1 },
			{ -1, -1,  0, -1,  0, -1, -1 },
			{ -1, -1, -1,  0, -1, -1, -1 },
			{ -1, -1,  0, -1,  0, -1, -1 },
			{ -1,  0, -1, -1, -1,  0, -1 },
			{  0, -1, -1, -1, -1, -1,  0 },
		},
		0,
		{0, 0},
		{}
	},
	{
		{
			{  0, -1, -1,  1, -1, -1,  0 },
			{ -1,  0, -1, -1, -1,  0, -1 },
			{ -1, -1,  0, -1,  0, -1, -1 },
			{  1, -1, -1,  0, -1, -1,  1 },
			{ -1, -1,  0, -1,  0, -1, -1 },
			{ -1,  0, -1, -1, -1,  0, -1 },
			{  0, -1, -1,  1, -1, -1,  0 },
		},
		0,
		{0, 0},
		{}
	},
	{
		{
			{  0,  0,  0,  0,  0,  0,  0 },
			{  0, -1, -1, -1, -1, -1,  0 },
			{  0, -1, -1, -1, -1, -1,  0 },
			{  0, -1, -1, -1, -1, -1,  0 },
			{  0, -1, -1, -1, -1, -1,  0 },
			{  0, -1, -1, -1, -1, -1,  0 },
			{  0,  0,  0,  0,  0,  0,  0 },
		},
		0,
		{0, 0},
		{}
	},
	{
		{
			{  0,  0,  0,  0,  0,  0,  0 },
			{  0, -1, -1,  1, -1, -1,  0 },
			{  0, -1,  1,  0,  1, -1,  0 },
			{  0, -1, -1,  1, -1, -1,  0 },
			{  0, -1, -1,  0, -1, -1,  0 },
			{  0, -1, -1,  1, -1, -1,  0 },
			{  0,  0,  0,  0,  0,  0,  0 },
		},
		0,
		{0, 0},
		{}
	},
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1,  0,  0,  0,  0, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1,  1,  1,  1,  1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
		},
		0,
		{0, 0},
		{}
	},
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1,  0,  0,  0, -1, -1 },
			{ -1, -1, -1, -1,  1, -1, -1 },
			{ -1, -1, -1, -1,  1, -1, -1 },
			{ -1, -1, -1, -1,  1, -1, -1 },
			{ -1, -1, -1, -1,  1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
		},
		0,
		{0, 0},
		{}
	},
};

std::vector<Tests::MinmaxParams> Tests::_minmaxTestCases = {
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1,  0 },
			{ -1, -1,  0,  0, -1,  1, -1 },
			{ -1, -1,  0,  0,  1, -1, -1 },
			{ -1,  1, -1,  1,  1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
		},
		0,
		{ 0, 0 },
		{},
	},
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1,  0,  1,  1, -1, -1 },
			{ -1, -1,  0,  0,  1, -1, -1 },
			{ -1, -1,  0, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
		},
		0,
		{ 0, 0 },
		{ 3 },
	},
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1,  0, -1, -1 },
			{ -1, -1,  0,  0,  1, -1, -1 },
			{ -1, -1,  0,  0,  1, -1, -1 },
			{ -1, -1, -1,  1,  1, -1, -1 },
			{ -1, -1, -1, -1,  1, -1, -1 },
			{ -1, -1, -1, -1,  0, -1, -1 },
		},
		0,
		{ 0, 0 },
		{},
	},
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1,  1, -1, -1, -1, -1 },
			{ -1, -1,  0,  1, -1, -1, -1 },
			{ -1, -1,  0,  0,  1, -1, -1 },
			{ -1, -1,  0, -1,  1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
		},
		0,
		{ 0, 0 },
		{},
	},
	{
		{
			{ -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1,  0, -1, -1, -1, -1, -1,  1, -1, -1 },
			{ -1, -1, -1, -1,  0,  0, -1,  1, -1,  0,  0, -1, -1 },
			{ -1, -1, -1,  0, -1,  1,  0,  0,  0,  1,  0, -1, -1 },
			{ -1, -1,  1,  0,  1,  1,  1,  0, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1,  0,  0,  1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1,  0,  1,  0, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1,  0,  0,  1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		},
		0,
		{ 0, 0 },
		{},
	},
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1,  1,  0,  0,  0,  0, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		},
		0,
		{ 0, 0 },
		{},
	},
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1,  0,  0,  0,  0, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1,  1,  1,  1,  1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
		},
		0,
		{0, 0},
		{ 2 },
	},
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1,  0,  0,  0, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1,  1,  1,  1,  1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
		},
		0,
		{0, 0},
		{ 4 },
	},
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1,  0,  0,  0, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1,  1,  1,  1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
		},
		0,
		{0, 0},
		{ 4 },
	},
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1, -1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1,  0, -1,  0, -1,  0, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1,  1,  0, -1, -1,  0,  0,  1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1,  1, -1, -1,  1, -1,  0,  1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1,  1,  1,  0, -1,  0, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1,  1,  0,  0,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1,  1,  0, -1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
			{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		},
		1,
		{4, 2},
		{ 4 },
	},
	{
		{
			{ -1, -1, -1, -1, -1, -1, -1 },
			{ -1,  0,  0,  0,  0, -1, -1 },
			{ -1,  1,  1,  1,  1, -1, -1 },
			{ -1, -1, -1, -1,  1, -1, -1 },
			{ -1, -1, -1, -1,  1, -1, -1 },
			{ -1, -1, -1, -1,  1, -1, -1 },
			{ -1, -1, -1, -1, -1, -1, -1 },
		},
		0,
		{0, 0},
		{ 3 },
	},
};