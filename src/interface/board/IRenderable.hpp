#pragma once

#include "Common.hpp"

class IRenderable {

public:
	virtual void render(sf::RenderWindow&) = 0;
	
	virtual void click(sf::Vector2i mousePosition) = 0;
	virtual void hover(sf::Vector2i mousePosition) = 0;

	std::vector<std::function<void(sf::Vector2i mousePosition)>> clickCallbacks;
	std::vector<std::function<void(sf::Vector2i mousePosition)>> hoverCallbacks;


	void setBorder(float thickness, sf::Color color) {
		_shape->setOutlineThickness(thickness);
		_shape->setOutlineColor(color);
	}

	void setBackgroundColor(sf::Color color) {
		_shape->setFillColor(color);
	}

protected:
	std::shared_ptr<sf::Shape> _shape;
	sf::Vector2f _size;
	sf::Vector2f _position;
};