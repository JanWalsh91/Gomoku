#pragma once

#include "Common.hpp"

class Button: public IRenderable {

public:
	Button(sf::Vector2f size, sf::Vector2f position, sf::Color backgroundColor);

	void render(sf::RenderWindow&);
	void click(sf::Vector2i mousePosition);
	void hover(sf::Vector2i mousePosition);

	void setText(const std::string& text);
	void setFontColor(sf::Color color);

private:
	sf::Text _text;
};