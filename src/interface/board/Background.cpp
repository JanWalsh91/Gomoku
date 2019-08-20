#include "Background.hpp"

Background::Background(sf::Vector2f size, sf::Vector2f position, sf::Color backgroundColor) {
	_position = position;
	_size = size;

	_shape = std::make_shared<sf::RectangleShape>(size);
	_shape->setFillColor(backgroundColor);
	_shape->move(position.x, position.y);
}

void Background::render(sf::RenderWindow& window) {
	window.draw(*_shape);
}

void Background::click(sf::Vector2i mousePosition) {
	(void)mousePosition;
}

void Background::hover(sf::Vector2i mousePosition) {
	(void)mousePosition;
}

void Background::setTexture(std::string pathToTexture) {
	if (_texture.loadFromFile(pathToTexture)) {
		_shape->setTexture(&_texture);
		_shape->setFillColor(sf::Color::White);
		_shape->setTextureRect(sf::IntRect(0, 0, 1000, 1000));
	}
}