#include "Timer.hpp"

void Timer::Start(std::string name) {
	if (Timer::_timers.find(name) == Timer::_timers.end()) {
		Timer::_timers[name] = std::vector<std::pair<std::chrono::high_resolution_clock::time_point , std::chrono::high_resolution_clock::time_point >>();
	}

	Timer::_timers[name].push_back(std::make_pair(std::chrono::high_resolution_clock::now(), std::chrono::high_resolution_clock::now()));
}

void Timer::Stop(std::string name) {
	Timer::_timers[name][Timer::_timers[name].size() - 1].second = std::chrono::high_resolution_clock::now();
}

void Timer::Clear(std::string name) {
	Timer::_timers.erase(name);
}

void Timer::Print(std::string name, int depth, bool sum) {

	double total = 0.0;
	for (size_t i = 0; i < Timer::_timers[name].size(); i++) {
		total += std::chrono::duration_cast<std::chrono::microseconds>(Timer::_timers[name][i].second - Timer::_timers[name][i].first).count();
	}

	for (int i = 0; i < depth; i++) {
		std::cout << "  ";
	}

	if (sum) {
		std::cout << "\u001b[36mTimer::" << name << ": " << total / 1000.0 << " ms (" << Timer::_timers[name].size() << ")\u001b[0m" << std::endl;
	} else {
		std::cout << "\u001b[36mTimer::" << name << ": " << (total / Timer::_timers[name].size()) / 1000.0 << " ms (" << Timer::_timers[name].size() << ")\u001b[0m" << std::endl;
	}
}

std::map<std::string, std::vector<std::pair<std::chrono::high_resolution_clock::time_point , std::chrono::high_resolution_clock::time_point >>> Timer::_timers;
