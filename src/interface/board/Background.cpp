#include "Background.hpp"

Background::Background() {
	this->shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(800.0f, 790.0f));
	this->shape->setFillColor(sf::Color(39, 174, 96));
	this->shape->setOutlineThickness(5.0f);
	this->shape->setOutlineColor(sf::Color(200, 200, 200));
	this->shape->move(5.0f, 5.0f);
}

void Background::render(sf::RenderWindow& window) {
	window.draw(*this->shape);
}