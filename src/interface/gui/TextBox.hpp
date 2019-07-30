#pragma once

#include "Common.hpp"

class TextBox : public IRenderable {

public:
	TextBox(float width, float height, float xPos, float yPos, sf::Color backgroundColor);

	void render(sf::RenderWindow&);
	void click(sf::Vector2i mousePosition);
	void hover(sf::Vector2i mousePosition);

	void setText(const std::string& text);
	void setFontColor(sf::Color color);
	void setFontSize(unsigned fontSize);
	
	static void loadFont();
	static sf::Font font;

private:
	sf::Text text;

	sf::Vector2f size; // TODO: in IRenderable?
	sf::Vector2f pos;
};