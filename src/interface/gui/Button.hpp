#pragma once

#include "Common.hpp"

class Button: public IRenderable {

public:
	Button(float width, float height, float xPos, float yPos, sf::Color backgroundColor);

	void render(sf::RenderWindow&);
	void click(sf::Vector2i mousePosition);
	void hover(sf::Vector2i mousePosition);

	void setText(const std::string& text);
	void setFontColor(sf::Color color);

private:
	sf::Text text;

	sf::Vector2f size; // TODO: in IRenderable?
	sf::Vector2f pos;
};