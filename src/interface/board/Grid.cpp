#include "Grid.hpp"

#include <iostream>

Grid::Grid(int nbCells, float boardSize, float margin) : _nbCells(nbCells), _boardSize(boardSize), _margin(margin) {

	_size = sf::Vector2f(boardSize, boardSize);
	_cellSize = (boardSize - _margin * 2) / (nbCells + 1);

	for (int i = 0; i < nbCells + 2; i++) {

		auto Hline = std::make_shared<sf::RectangleShape>(sf::Vector2f(boardSize - _margin * 2, 1));
		Hline->move(_margin, _margin + (i * _cellSize));

		auto Vline = std::make_shared<sf::RectangleShape>(sf::Vector2f(boardSize - _margin * 2, 1));
		Vline->move(_margin + (i * _cellSize), _margin);
		Vline->rotate(90.0f);

		_gridLines.push_back(Hline);
		_gridLines.push_back(Vline);
	}

	if (_blackStoneTexture.loadFromFile("resources/textures/black_stone.png")) {
		_blackStoneTexture.setSmooth(false);
	}
	if (_whiteStoneTexture.loadFromFile("resources/textures/white_stone.png")) {
		_whiteStoneTexture.setSmooth(false);
	}
	
}

/**
	Places a stone at the position pos. Pos reprensents the indices in the grid (0, 0), (1, 2), ...
*/
void Grid::placeStoneAt(std::pair<int, int> pos, sf::Color color) {

	if (pos.first < 0 || pos.second < 0 || pos.first >= _nbCells || pos.second >= _nbCells) {
		std::cerr << "Error, wrong position" << std::endl;
		return ;
	}

	auto stone = std::make_shared<sf::CircleShape>(_cellSize / 2.0f);

	stone->move(_margin + (pos.second * _cellSize) + _cellSize / 2.0f, _margin + (pos.first * _cellSize) + _cellSize / 2.0f);

	if (color == sf::Color::Black) {
		stone->setTexture(&_blackStoneTexture);
	}
	else {
		stone->setTexture(&_whiteStoneTexture);
	}
	stone->setTextureRect(sf::IntRect(0, 0, 545, 545));

	_stones[pos] = stone;
}

void Grid::removeStoneAt(std::pair<int, int> pos) {
	_stones.erase(pos);
}

std::pair<int, int> Grid::windowToGridCoord(sf::Vector2i mousePosition) {

	if (mousePosition.x < _margin * 1.5f || mousePosition.y < _margin * 1.5f || mousePosition.x > _boardSize - _margin * 1.5f || mousePosition.y > _boardSize - _margin * 1.5f) {
		return std::make_pair(-1, -1);
	}

	return std::make_pair(static_cast<int>((mousePosition.y / _cellSize) - _margin / _cellSize - 0.5f), static_cast<int>((mousePosition.x / _cellSize) - _margin / _cellSize - 0.5f));
}


void Grid::render(sf::RenderWindow& window) {
	for (auto& line : _gridLines) {
		window.draw(*line);
	}

	for (auto& stone : _stones) {
		window.draw(*stone.second);
	}
}

void Grid::click(sf::Vector2i mousePosition) {

	if (mousePosition.x < 800 && mousePosition.y < 800) {
		for (auto& callback : clickCallbacks) {
			callback(mousePosition);
		}
	}
}

void Grid::hover(sf::Vector2i mousePosition) {

	if (mousePosition.x < 800 && mousePosition.y < 800) {
		auto pos = this->windowToGridCoord(mousePosition);

		if (pos.first < 0 || pos.second < 0 || pos.first >= _nbCells || pos.second >= _nbCells || _stones.find(pos) != _stones.end()) {
			return;
		}
		
		float xPosition = _margin + (pos.first * _cellSize) + _cellSize / 2.0f;
		float yPosition = _margin + (pos.second * _cellSize) + _cellSize / 2.0f;

		for (auto& callback : hoverCallbacks) {
			callback(sf::Vector2i(static_cast<int>(xPosition), static_cast<int>(yPosition)));
		}
	}
}

float Grid::getCellSize() const {
	return _cellSize;
}

void Grid::reset() {
	_stones.clear(); // TODO: leaks? Check shared_ptr
}