#pragma once

#include "IRenderable.hpp"
#include <SFML/Graphics.hpp>

class Background: public IRenderable {

public:
	Background();
	void render(sf::RenderWindow&);

};