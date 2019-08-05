#pragma once

#include "Common.hpp"

std::ostream & operator << (std::ostream &out, std::pair<int, int> &c);

template <typename T>
std::pair<T, T> & operator + (const std::pair<T, T> &left, const std::pair<T, T> &right);

template <typename T>
std::pair<T, T> & operator - (const std::pair<T, T> &left, const std::pair<T, T> &right);

template <typename T>
std::pair<T, T> & operator * (const std::pair<T, T> &left, const std::pair<T, T> &right);