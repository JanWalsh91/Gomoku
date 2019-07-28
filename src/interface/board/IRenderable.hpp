#pragma once

#include <SFML/Graphics/Shape.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>

class IRenderable {

public:
	virtual void render(sf::RenderWindow&) = 0;
	
	virtual void click(sf::Vector2i mousePosition) = 0;
	virtual void hover(sf::Vector2i mousePosition) = 0;

	std::vector<std::function<void(sf::Vector2i mousePosition)>> callbacks;
	std::vector<std::function<void(sf::Vector2i mousePosition)>> hoverCallbacks;
protected:
	std::shared_ptr<sf::Shape> shape;
};