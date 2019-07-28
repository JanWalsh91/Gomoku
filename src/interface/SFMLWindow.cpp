#include "SFMLWindow.hpp"

#include <iostream>

SFMLWindow::SFMLWindow(int width, int height, const std::string& title): width(width), height(height), title(title) {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
	this->window.create(sf::VideoMode(width, height), title, sf::Style::Close, settings);
	
}

SFMLWindow::~SFMLWindow() {

}

void SFMLWindow::setupGUI(std::vector<std::shared_ptr<Player>>& players) {
	auto background = std::make_shared<Background>();
	auto grid = std::make_shared<Grid>(5, 800.0f, 40.0f);

	grid->callbacks.push_back([grid, players, this](sf::Vector2i mousePosition) mutable {
		black = !black;
		grid->placeStoneAt(grid->windowToGridCoord(mousePosition), black ? sf::Color::Black : sf::Color::White);
	});
	grid->hoverCallbacks.push_back([grid, this](sf::Vector2i mousePosition) mutable {
		
		//std::cout << "black: " << black << std::endl;

		auto circle = sf::CircleShape(grid->getCellSize() / 2.0f);
		circle.setFillColor(black ? sf::Color(50, 50, 50, 125) : sf::Color(200, 200, 200, 125));
		circle.setOutlineThickness(1.0f);
		circle.setOutlineColor(sf::Color::Black);
		circle.setPosition(mousePosition.y, mousePosition.x);
		this->getWindow()->draw(circle);
	});

	this->renderables.push_back(background);
	this->renderables.push_back(grid);

}

void SFMLWindow::loop() {

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}

			if (event.type == sf::Event::EventType::MouseButtonPressed) {
				for (auto& renderable : renderables) {
					renderable->click(sf::Mouse::getPosition(this->window));
				}
			}
		}


		window.clear(sf::Color::Black);

		for (auto& renderable : renderables) {
			renderable->render(window);
			renderable->hover(sf::Mouse::getPosition(this->window));
		}

		window.display();
	}
}

sf::RenderWindow* SFMLWindow::getWindow() {
	return &this->window;
}