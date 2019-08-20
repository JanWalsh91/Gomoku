#include "Button.hpp"

#include <iostream>

Button::Button(sf::Vector2f size, sf::Vector2f position, sf::Color backgroundColor) {

	_position = position;
	_size = size;

	_shape = std::make_shared<sf::RectangleShape>(size);
	_shape->setPosition(position);
	_shape->setFillColor(backgroundColor);

	_text.setFont(TextBox::font);

	sf::FloatRect textRect = _text.getLocalBounds();
	_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	_text.setPosition(sf::Vector2f(position.x + size.x / 2.0f, position.y + size.y / 2.0f));

	_text.setPosition(position.x, position.y + size.y / 2.0f - 25 / 2.0f); // Take font size into account
	_text.setCharacterSize(25);
	_text.setStyle(sf::Text::Bold);
}

void Button::render(sf::RenderWindow& window) {
	window.draw(*_shape);
	window.draw(_text);
}

void Button::click(sf::Vector2i mousePosition) {
	
	if (mousePosition.x > _position.x && mousePosition.x < _position.x + _size.x && mousePosition.y > _position.y && mousePosition.y < _position.y + _size.y) {
		for (auto& callback : clickCallbacks) {
			callback(mousePosition);
		}
	}
}

void Button::hover(sf::Vector2i mousePosition) {
	(void)mousePosition;
}

void Button::setFontColor(sf::Color color) {
	_text.setFillColor(color);
}

void Button::setText(const std::string& buttonText) {
	_text.setString(buttonText);
	sf::FloatRect textRect = _text.getLocalBounds();
	_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	_text.setPosition(sf::Vector2f(_position.x + _size.x / 2.0f, _position.y + _size.y / 2.0f));
}
