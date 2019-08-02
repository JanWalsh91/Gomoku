#include "Background.hpp"

Background::Background(float width, float height, float xPos, float yPos, sf::Color backgroundColor) {

	this->shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(width, height));
	this->shape->setFillColor(backgroundColor);
	this->shape->move(xPos, yPos);
}

void Background::render(sf::RenderWindow& window) {
	window.draw(*this->shape);
}

void Background::click(sf::Vector2i mousePosition) {

}

void Background::hover(sf::Vector2i mousePosition) {

}

void Background::setTexture(std::string pathToTexture) {
	if (_texture.loadFromFile(pathToTexture)) {
		this->shape->setTexture(&_texture);
		this->shape->setFillColor(sf::Color::White);
		this->shape->setTextureRect(sf::IntRect(0, 0, 1000, 1000));

	}
}