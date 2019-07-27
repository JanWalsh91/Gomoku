#include "ARule.hpp"
#include <algorithm>

class NoDoubleFreeThree: public ARule {
public:
	NoDoubleFreeThree();
	virtual bool canPlace(Gomoku& gomoku, std::pair<int, int>& move);
};
