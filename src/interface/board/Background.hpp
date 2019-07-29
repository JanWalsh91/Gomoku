#pragma once

#include "IRenderable.hpp"
#include <SFML/Graphics.hpp>

class Background: public IRenderable {

public:
	Background(float width, float height, float xPos, float yPos, sf::Color backgroundColor);

	void render(sf::RenderWindow&);
	void click(sf::Vector2i mousePosition);
	void hover(sf::Vector2i mousePosition);
};