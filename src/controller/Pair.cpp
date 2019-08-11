#include "Pair.hpp"

std::ostream & operator << (std::ostream &out, std::pair<int, int> &c) {
    out << "[" << c.first << ", " << c.second << "]";
    return out;
}

std::pair<int, int> operator + (const std::pair<int, int> &left, const std::pair<int, int> &right) {
	return std::make_pair(
		left.first + right.first,
		left.second + right.second
	);
}

std::pair<int, int> operator - (const std::pair<int, int> &left, const std::pair<int, int> &right) {
	return std::make_pair(
		left.first - right.first,
		left.second - right.second
	);
}

std::pair<int, int> operator * (const std::pair<int, int> &left, const std::pair<int, int> &right) {
	return std::make_pair(
		left.first * right.first,
		left.second * right.second
	);
}

std::pair<int, int> operator * (const std::pair<int, int> &left, int right) {
	return std::make_pair(
		left.first * right,
		left.second * right
	);
}

std::pair<int, int> & operator += (std::pair<int, int> &left, const std::pair<int, int> &right) {
	left.first += right.first;
	left.second += right.second;
	return left;
}
