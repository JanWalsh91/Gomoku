#include "SFMLWindow.hpp"

SFMLWindow::SFMLWindow(int width, int height, const std::string& title): width(width), height(height), title(title) {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
	this->window.create(sf::VideoMode(width, height), title, sf::Style::Default, settings);
	

	this->renderables.push_back(std::make_shared<Background>());
	this->renderables.push_back(std::make_shared<Grid>());
}

SFMLWindow::~SFMLWindow() {

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
		}


		window.clear(sf::Color::Black);

		for (auto& renderable : renderables) {
			renderable->render(window);
		}

		window.display();
	}
}