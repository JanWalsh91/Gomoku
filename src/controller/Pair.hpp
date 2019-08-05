#pragma once

#include "Common.hpp"

std::ostream & operator << (std::ostream &out, std::pair<int, int> &c);

std::pair<int, int> operator + (const std::pair<int, int> &left, const std::pair<int, int> &right);

std::pair<int, int> operator - (const std::pair<int, int> &left, const std::pair<int, int> &right);

std::pair<int, int> operator * (const std::pair<int, int> &left, const std::pair<int, int> &right);

std::pair<int, int> operator * (const std::pair<int, int> &left, int right);

std::pair<int, int> & operator += (std::pair<int, int> &left, const std::pair<int, int> &right);
