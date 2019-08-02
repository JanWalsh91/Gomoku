#include "SFMLWindow.hpp"

#include <iostream>

SFMLWindow::SFMLWindow(int width, int height, const std::string& title): width(width), height(height), title(title) {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	
	this->window.create(sf::VideoMode(width, height), title, sf::Style::Close, settings);
}

SFMLWindow::~SFMLWindow() {

}

void SFMLWindow::setupGUI(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Gomoku> gomoku) {
	


}

void SFMLWindow::loop(std::function<void()> f, std::function<void(sf::Event)> e) {

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
			e(event);
		}

		f();

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

void SFMLWindow::addRenderable(std::shared_ptr<IRenderable> renderable) {
	this->renderables.push_back(renderable);
}