#pragma once

#include "Common.hpp"

class Timer {

public:

	static void Start(std::string);
	static void Clear(std::string);
	static void Stop(std::string);
	static void Print(std::string, int depth = 0, bool sum = false);
	static double Get(std::string, int depth = 0, bool sum = false);


private:

	static std::map<std::string, std::vector<std::pair<std::chrono::high_resolution_clock::time_point , std::chrono::high_resolution_clock::time_point >>> _timers;

};