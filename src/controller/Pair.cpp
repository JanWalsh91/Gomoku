#include "Pair.hpp"

std::ostream & operator << (std::ostream &out, std::pair<int, int> &c) {
    out << "[" << c.first << ", " << c.second << "]";
    return out;
}

template <typename T>
std::pair<int, int> operator + (const std::pair<int, int> &left, const std::pair<int, int> &right) {
	return {
		left.first + right.first,
		left.second + right.second,
	};
}

template <typename T>
std::pair<int, int> operator - (const std::pair<int, int> &left, const std::pair<int, int> &right) {
	return {
		left.first - right.first,
		left.second - right.second,
	};
}

template <typename T>
std::pair<int, int> operator * (const std::pair<int, int> &left, const std::pair<int, int> &right) {
	return {
		left.first * right.first,
		left.second * right.second,
	};
}