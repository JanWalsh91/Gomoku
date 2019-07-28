#pragma once

#include <SFML/Graphics/Shape.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

class IRenderable {

public:
	virtual void render(sf::RenderWindow&) = 0;

protected:
	std::shared_ptr<sf::Shape> shape;
};