#include "TextBox.hpp"

#include <iostream>

TextBox::TextBox(sf::Vector2f size, sf::Vector2f position, sf::Color backgroundColor) {

	_position = position;
	_size = size;

	_shape = std::make_shared<sf::RectangleShape>(size);
	_shape->setPosition(position);
	_shape->setFillColor(backgroundColor);

	_text.setFont(TextBox::font);

	sf::FloatRect textRect = _text.getLocalBounds();
	_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	_text.setPosition(sf::Vector2f(position.x + size.x / 2.0f, position.y + size.y / 2.0f));

	_text.setPosition(position.x, position.y + size.y / 2.0f - 25 / 2.0f);
	_text.setCharacterSize(25);
}

void TextBox::render(sf::RenderWindow& window) {
	window.draw(*_shape);
	window.draw(_text);
}

void TextBox::click(sf::Vector2i mousePosition) {
	(void)mousePosition;
	return;
}

void TextBox::hover(sf::Vector2i mousePosition) {
	(void)mousePosition;
}

void TextBox::setFontColor(sf::Color color) {
	_text.setFillColor(color);
}

void TextBox::setText(const std::string& buttonText) {
	_text.setString(buttonText);
	sf::FloatRect textRect = _text.getLocalBounds();
	_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	_text.setPosition(sf::Vector2f(_position.x + _size.x / 2.0f, _position.y + _size.y / 2.0f));
}

void TextBox::setFontSize(unsigned fontSize) {
	_text.setCharacterSize(fontSize);
	sf::FloatRect textRect = _text.getLocalBounds();
	_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	_text.setPosition(sf::Vector2f(_position.x + _size.x / 2.0f, _position.y + _size.y / 2.0f));
}

sf::Font TextBox::font;

void TextBox::loadFont() {
	if (!TextBox::font.loadFromFile("./resources/fonts/Catamaran-Regular.ttf")) {
		std::cerr << "Error while loading font" << std::endl;
	}
}
