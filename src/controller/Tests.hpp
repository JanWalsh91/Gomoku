#pragma once

class Tests {

public:
	static void init();

	static void runEvalLine();
	static void runEvalLine(int index);

	static void runHeuristic();
	static void runHeuristic(int index);

	static void runMinmax();
	static void runMinmax(int index);


private:

	typedef struct {
		std::vector<int> line;
	} EvalLineParams;

	typedef struct {
		std::vector<std::vector<int>> board;
		int currentPlayer;
		int capturesPerPlayer[2];
		std::vector<std::pair<int, int>> moves;
	} HeuristicParams;

	typedef struct {
		std::vector<std::vector<int>> board;
		int currentPlayer;
		int capturesPerPlayer[2];
		std::vector<int> depths;
	} MinmaxParams;


	static void _evalLine(EvalLineParams& param);
	static void _heuristic(HeuristicParams& param);
	static void _minmaxUnitTest(MinmaxParams& param);

	static std::shared_ptr<Gomoku> _gomoku;
	static std::shared_ptr<Minmax> _minmax;
	
	static int _DefaultSize;
	static int _DefaultDepth;

	static std::vector<EvalLineParams> _evalLineTestCases;
	static std::vector<HeuristicParams> _heuristicTestCases;
	static std::vector<MinmaxParams> _minmaxTestCases;
};