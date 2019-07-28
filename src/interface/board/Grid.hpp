#pragma once

#include "IRenderable.hpp"

#include <vector>
#include <memory>

class Grid: public IRenderable {

public:

	Grid();
	void render(sf::RenderWindow&);

private:
	std::vector<std::shared_ptr<sf::Shape>> gridLines;
};