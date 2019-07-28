#pragma once

#include "IRenderable.hpp"
#include <SFML/Graphics.hpp>

class Background: public IRenderable {

public:
	Background();
	void render(sf::RenderWindow&);
	void click(sf::Vector2i mousePosition);
	void hover(sf::Vector2i mousePosition);
};