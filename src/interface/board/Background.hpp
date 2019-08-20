#pragma once

#include "Common.hpp"

class Background: public IRenderable {

public:
	Background(sf::Vector2f size, sf::Vector2f position, sf::Color backgroundColor);

	void render(sf::RenderWindow&);
	void click(sf::Vector2i mousePosition);
	void hover(sf::Vector2i mousePosition);

	void setTexture(std::string pathToTexture);

private:

	sf::Texture _texture;
};