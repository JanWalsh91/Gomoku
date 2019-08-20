#pragma once

#include "Common.hpp"

class Grid: public IRenderable {

public:

	Grid(int nbCells, float boardSize, float margin);
	void render(sf::RenderWindow&);
	void click(sf::Vector2i mousePosition);
	void hover(sf::Vector2i mousePosition);

	void placeStoneAt(std::pair<int, int> pos, sf::Color color);
	void removeStoneAt(std::pair<int, int> pos);
	std::pair<int, int> windowToGridCoord(sf::Vector2i mousePosition);

	float getCellSize() const;

	void reset();

private:
	std::vector<std::shared_ptr<sf::Shape>> _gridLines;
	std::map<std::pair<int, int>, std::shared_ptr<sf::Shape>> _stones;
	
	int _nbCells;
	float _boardSize;
	float _cellSize;
	float _margin;

	sf::Texture _blackStoneTexture;
	sf::Texture _whiteStoneTexture;

};