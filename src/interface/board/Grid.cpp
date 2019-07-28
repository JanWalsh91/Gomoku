#include "Grid.hpp"

Grid::Grid() {

	// width = 800 - 10 (borders) - 40 (margin)

	const float boardSize = 800.0f;
	const float border = 5.0f;
	const float margin = 20.0f;

	const int nbCells = 5;
	const float cellSize = (boardSize - border * 2 - margin * 2) / nbCells;


	for (int i = 0; i < nbCells + 2; i++) {

		auto Hline = std::make_shared<sf::RectangleShape>(sf::Vector2f(boardSize - margin * 2, 1));
		Hline->move(border + margin, border + margin + (i * cellSize));

		auto Vline = std::make_shared<sf::RectangleShape>(sf::Vector2f(boardSize - margin * 2, 1));
		Vline->move(border + margin + (i * cellSize), border + margin);
		Vline->rotate(90.0f);

		gridLines.push_back(Hline);
		gridLines.push_back(Vline);
	}

}

void Grid::render(sf::RenderWindow& window) {
	for (auto& line : gridLines) {
		window.draw(*line);
	}
}