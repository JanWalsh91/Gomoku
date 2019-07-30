#pragma once

#include "Common.hpp"

class IRenderable {

public:
	virtual void render(sf::RenderWindow&) = 0;
	
	virtual void click(sf::Vector2i mousePosition) = 0;
	virtual void hover(sf::Vector2i mousePosition) = 0;

	std::vector<std::function<void(sf::Vector2i mousePosition)>> callbacks;
	std::vector<std::function<void(sf::Vector2i mousePosition)>> hoverCallbacks;


	void setBorder(float thickness, sf::Color color) {
		this->shape->setOutlineThickness(thickness);
		this->shape->setOutlineColor(color);
	}

	void setBackgroundColor(sf::Color color) {
		this->shape->setFillColor(color);
	}

protected:
	std::shared_ptr<sf::Shape> shape;
};