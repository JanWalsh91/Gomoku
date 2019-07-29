#include "Button.hpp"

#include <iostream>

Button::Button(float width, float height, float xPos, float yPos, sf::Color backgroundColor): size(sf::Vector2f(width, height)), pos(sf::Vector2f(xPos, yPos)) {

	this->shape = std::make_shared<sf::RectangleShape>(size);
	this->shape->setPosition(pos);
	this->shape->setFillColor(backgroundColor);

	this->text.setFont(TextBox::font);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(pos.x + width / 2.0f, pos.y + height / 2.0f));

	this->text.setPosition(pos.x, pos.y + size.y / 2.0f - 25 / 2.0f); // Take font size into account
	this->text.setCharacterSize(25);
	this->text.setStyle(sf::Text::Bold);
}

void Button::render(sf::RenderWindow& window) {
	window.draw(*this->shape);
	window.draw(this->text);
}

void Button::click(sf::Vector2i mousePosition) {
	
	if (mousePosition.x > pos.x && mousePosition.x < pos.x + size.x && mousePosition.y > pos.y && mousePosition.y < pos.y + size.y) {
		for (auto& callback : callbacks) {
			callback(mousePosition);
		}
		std::cout << "Clicked on button" << std::endl;
	}
}

void Button::hover(sf::Vector2i mousePosition) {

}

void Button::setFontColor(sf::Color color) {
	this->text.setFillColor(color);
}

void Button::setText(const std::string& buttonText) {
	this->text.setString(buttonText);
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f));
}
