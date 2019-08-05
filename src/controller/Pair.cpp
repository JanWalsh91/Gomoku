#include "Pair.hpp"

std::ostream & operator << (std::ostream &out, const std::pair<int, int> &c) {
    out << "[" << c.first << ", " << c.second << "]";
    return out;
}

std::pair<int, int> operator + (const std::pair<int, int> & l, const std::pair<int, int> & r) {   
	return std::make_pair(l.first + r.first, l.second + r.second);                                    
}

std::pair<int, int> operator - (const std::pair<int, int> & l, const std::pair<int, int> & r) {   
	return std::make_pair(l.first - r.first, l.second - r.second);                                    
}

std::pair<int, int> operator * (const std::pair<int, int> & l, const std::pair<int, int> & r) {   
	return std::make_pair(l.first * r.first, l.second * r.second);                                    
}
