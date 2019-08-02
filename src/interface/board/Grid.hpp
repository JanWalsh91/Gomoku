#pragma once

#include "Common.hpp"

class Grid: public IRenderable {

public:

	Grid(int size, float boardSize, float margin);
	void render(sf::RenderWindow&);
	void click(sf::Vector2i mousePosition);
	void hover(sf::Vector2i mousePosition);

	bool placeStoneAt(std::pair<int, int> pos, sf::Color color);
	std::pair<int, int> windowToGridCoord(sf::Vector2i mousePosition);

	float getCellSize() const;

	void reset();

private:
	std::vector<std::shared_ptr<sf::Shape>> gridLines;
	std::map<std::pair<int, int>, std::shared_ptr<sf::Shape>> stones;
	
	int size;
	float boardSize;
	float cellSize;
	float margin;

	sf::Texture _blackStoneTexture;
	sf::Texture _whiteStoneTexture;

};