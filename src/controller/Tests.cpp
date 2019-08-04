#include "Common.hpp"

void Tests::init() {
	Tests::_gomoku->minmax = Tests::_minmax;
}

// === EVAL LINE === //

void Tests::runEvalLine(int index) {
	if (index == -1) {
		Tests::runEvalLine(static_cast<int>(Tests::_evalLineTestCases.size() - 1));
	} 
	Tests::_evalLine(Tests::_evalLineTestCases[index]);

}

void Tests::runEvalLine() {
	for (size_t i = 0; i < Tests::_evalLineTestCases.size(); i++) {
		Tests::runEvalLine(i);
	}
}

void Tests::_evalLine(EvalLineParams& param) {
	std::cout << " === Unit Test: Eval Line " << std::endl;

	Tests::_gomoku->size = param.line.size();
	Tests::_gomoku->reset();
	Tests::_gomoku->board[0] = param.line;

	int potentialCaptures = 0;
	int score = Tests::_gomoku->evalLine(std::make_pair(0, 0), std::make_pair(0, 1), 0, Tests::_gomoku->size, potentialCaptures);

	for (int i = 0; i < Tests::_gomoku->size; i ++) {
		if (i == Tests::_gomoku->size - 1) {
			std::cout << Tests::_gomoku->board[0][i];
		} else {
			std::cout << Tests::_gomoku->board[0][i] << ", ";
		}
	}
	std::cout << "\t => score: " << score << ", potentialCaptures: " << potentialCaptures << std::endl;
}

// === HEURISTIC === //

void Tests::runHeuristic(int index) {
	if (index == -1) {
		Tests::runHeuristic(static_cast<int>(Tests::_heuristicTestCases.size() - 1));
	} 
	Tests::_heuristic(Tests::_heuristicTestCases[index]);
}

void Tests::runHeuristic() {
	for (size_t i = 0; i < Tests::_heuristicTestCases.size(); i++) {
		Tests::runHeuristic(i);
	}
}

void Tests::_heuristic(HeuristicParams& param) {
	std::cout << " === Unit Test: Heuristic " << std::endl;

	Tests::_gomoku->size = param.board.size();
	Tests::_gomoku->reset();
	Tests::_gomoku->board = param.board;


	Tests::_gomoku->currentPlayer = Tests::_gomoku->heuristicPlayer = Tests::_gomoku->players[param.currentPlayer];

	auto heuristicValues = std::vector<std::vector<int>>(Tests::_gomoku->size, std::vector<int>(Tests::_gomoku->size, 0));
	
	std::vector<std::pair<int, int>> moves;

	if (param.moves.size()) {
		moves = param.moves;
	} else {
		moves = Tests::_gomoku->getMoves();
	}

	int bestValue = Minmax::INF_MIN; // TODO -1 ?
	std::pair<int, int> bestMove;

	for (int i = 0; i < param.capturesPerPlayer[0]; i++) {
		Tests::_gomoku->players[0]->incrementCaptures();
	}
	for (int i = 0; i < param.capturesPerPlayer[1]; i++) {
		Tests::_gomoku->players[1]->incrementCaptures();
	}

	for (auto &move: moves) {
		auto undoMoves = Tests::_gomoku->doMove(move);
		// std::cout << "move: [" << move.first << ", " << move.second << "]" << std::endl << std::endl;   

		int ret = Tests::_gomoku->heuristic();
		
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
	} 
	Tests::_minmaxUnitTest(Tests::_minmaxTestCases[index]);
}

void Tests::runMinmax() {
	for (size_t i = 0; i < Tests::_minmaxTestCases.size(); i++) {
		Tests::runMinmax(i);
	}
}

void Tests::_minmaxUnitTest(MinmaxParams& param) {
	std::cout << " === Unit Test: Minmax " << std::endl;

	Tests::_gomoku->size = param.board.size();
	Tests::_gomoku->reset();
	Tests::_gomoku->board = param.board;


	Tests::_gomoku->currentPlayer = Tests::_gomoku->heuristicPlayer = Tests::_gomoku->players[param.currentPlayer];

	auto heuristicValues = std::vector<std::vector<int>>(Tests::_gomoku->size, std::vector<int>(Tests::_gomoku->size, 0));
	
	std::vector<std::pair<int, int>> moves;

	if (param.moves.size()) {
		moves = param.moves;
	} else {
		moves = Tests::_gomoku->getMoves();
	}

	std::vector<int> depths;

	if (param.depths.size() > 0) {
		depths = param.depths;
	} else {
		depths = { Tests::_DefaultDepth };
	}

	for (int& depth : depths ) {
		Tests::_minmax->maxDepth = depth;
		Tests::_gomoku->printBoard();
		std::cout << "===== DEPTH " << depth << " =====" << std::endl;
		Tests::_minmax->run();
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
		{},
		{},
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
		{},
	},
};