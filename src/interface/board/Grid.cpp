#include "Grid.hpp"

#include <iostream>

Grid::Grid(int size, float boardSize, float margin) : size(size), boardSize(boardSize), margin(margin) {

	this->cellSize = (boardSize - margin * 2) / (size + 1);

	for (int i = 0; i < size + 2; i++) {

		auto Hline = std::make_shared<sf::RectangleShape>(sf::Vector2f(boardSize - margin * 2, 1));
		Hline->move(margin, margin + (i * cellSize));

		auto Vline = std::make_shared<sf::RectangleShape>(sf::Vector2f(boardSize - margin * 2, 1));
		Vline->move(margin + (i * cellSize), margin);
		Vline->rotate(90.0f);

		gridLines.push_back(Hline);
		gridLines.push_back(Vline);
	}

	if (_blackStoneTexture.loadFromFile("resources/textures/black_stone.png")) {
		std::cout << "Set smooth" << std::endl;
		_blackStoneTexture.setSmooth(false);
	}
	if (_whiteStoneTexture.loadFromFile("resources/textures/white_stone.png")) {
		std::cout << "Set smooth" << std::endl;
		_whiteStoneTexture.setSmooth(false);
	}
	
}

/**
	Places a stone at the position pos. Pos reprensents the indices in the grid (0, 0), (1, 2), ...
*/
bool Grid::placeStoneAt(std::pair<int, int> pos, sf::Color color) {

	if (stones.find(pos) != stones.end()) {
		std::cout << "Error, already stone here" << std::endl;
		return false;
	}
	if (pos.first < 0 || pos.second < 0 || pos.first >= size || pos.second >= size) {
		std::cout << "Error, wrong position" << std::endl;
		return false;
	}

	auto stone = std::make_shared<sf::CircleShape>(cellSize / 2.0f);
	//stone->move(margin + (pos.second * cellSize), margin + (pos.first * cellSize));
	stone->move(margin + (pos.second * cellSize) + cellSize / 2.0f, margin + (pos.first * cellSize) + cellSize / 2.0f);
	if (color == sf::Color::Black) {
		stone->setTexture(&_blackStoneTexture);
	}
	else {
		stone->setTexture(&_whiteStoneTexture);
	}
	stone->setTextureRect(sf::IntRect(0, 0, 545, 545));

	//stone->move(margin + cellSize / 2.0f + (pos.second * cellSize), margin + cellSize / 2.0f + (pos.first * cellSize));
	//stone->setFillColor(color);

	stones[pos] = stone;
	return true;
}

std::pair<int, int> Grid::windowToGridCoord(sf::Vector2i mousePosition) {

	if (mousePosition.x < margin * 1.5f || mousePosition.y < margin * 1.5f || mousePosition.x > boardSize - margin * 1.5f || mousePosition.y > boardSize - margin * 1.5f) {
		return std::make_pair(-1, -1);
	}

	// move away from the margin

	//mousePosition.x + margin

	//std::cout << "World Position: " << mousePosition.x << ", " << mousePosition.y << std::endl;
	//auto pair = std::make_pair(static_cast<int>(std::round(mousePosition.y / cellSize)), static_cast<int>(std::round(mousePosition.x / cellSize)));
	auto pair = std::make_pair(static_cast<int>((mousePosition.y / cellSize) - margin / cellSize - 0.5f), static_cast<int>((mousePosition.x / cellSize) - margin / cellSize - 0.5f));
	//std::cout << "Grid Position before cast: " << ((mousePosition.y / cellSize) - margin / cellSize) << ", " << ((mousePosition.x / cellSize) - margin / cellSize) << std::endl;
	//std::cout << "Grid Position: " << pair.first << ", " << pair.second << std::endl;
	//std::cout << "cellSize: " << cellSize << std::endl;
	return pair;
}


void Grid::render(sf::RenderWindow& window) {
	for (auto& line : gridLines) {
		window.draw(*line);
	}

	for (auto& stone : stones) {
		window.draw(*stone.second);
	}
}

void Grid::click(sf::Vector2i mousePosition) {

	if (mousePosition.x < 800 && mousePosition.y < 800) {
		for (auto& callback : callbacks) {
			callback(mousePosition);
		}
	}
}

void Grid::hover(sf::Vector2i mousePosition) {

	if (mousePosition.x < 800 && mousePosition.y < 800) {
		auto pos = this->windowToGridCoord(mousePosition);

		if (pos.first < 0 || pos.second < 0 || pos.first >= size || pos.second >= size || stones.find(pos) != stones.end()) {
			return;
		}
		
		float xPosition = margin + (pos.first * cellSize) + cellSize / 2.0f;
		float yPosition = margin + (pos.second * cellSize) + cellSize / 2.0f;

		for (auto& callback : hoverCallbacks) {
			callback(sf::Vector2i(static_cast<int>(xPosition), static_cast<int>(yPosition)));
		}
	}
}

float Grid::getCellSize() const {
	return this->cellSize;
}

void Grid::reset() {
	stones.clear(); // TODO: leaks? Check shared_ptr
	std::cout << "Cleard the stones: " << stones.size() << std::endl;
}