#pragma once

#include "Common.hpp"

std::ostream & operator << (std::ostream &out, const std::pair<int, int> &c);

std::pair<int, int> operator + (const std::pair<int, int> & l, const std::pair<int, int> & r);

std::pair<int, int> operator - (const std::pair<int, int> & l, const std::pair<int, int> & r);

std::pair<int, int> operator * (const std::pair<int, int> & l, const std::pair<int, int> & r);
