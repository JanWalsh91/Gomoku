#include "TextBox.hpp"

#include <iostream>

TextBox::TextBox(float width, float height, float xPos, float yPos, sf::Color backgroundColor) : size(sf::Vector2f(width, height)), pos(sf::Vector2f(xPos, yPos)) {

	this->shape = std::make_shared<sf::RectangleShape>(size);
	this->shape->setPosition(pos);
	this->shape->setFillColor(backgroundColor);

	this->text.setFont(TextBox::font);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(pos.x + width / 2.0f, pos.y + height / 2.0f));

	this->text.setPosition(pos.x, pos.y + size.y / 2.0f - 25 / 2.0f); // Take font size into account
	this->text.setCharacterSize(25);
	// this->text.setStyle(sf::Text::Bold);
}

void TextBox::render(sf::RenderWindow& window) {
	window.draw(*this->shape);
	window.draw(this->text);
}

void TextBox::click(sf::Vector2i mousePosition) {
	return;
}

void TextBox::hover(sf::Vector2i mousePosition) {

}

void TextBox::setFontColor(sf::Color color) {
	this->text.setFillColor(color);
}

void TextBox::setText(const std::string& buttonText) {
	this->text.setString(buttonText);
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f));
}

void TextBox::setFontSize(unsigned fontSize) {
	this->text.setCharacterSize(fontSize);
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f));
}

sf::Font TextBox::font;

void TextBox::loadFont() {
	// if (!TextBox::font.loadFromFile("../resources/Roboto-Regular.ttf")) {
	if (!TextBox::font.loadFromFile("./resources/fonts/Catamaran-Regular.ttf")) {
		std::cout << "Error while loading font" << std::endl;
	}
}
