#pragma once

#include <string>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <memory>
#include <list>

#include "IRenderable.hpp"
#include "Background.hpp"
#include "Grid.hpp"

#include "Player.hpp"

class SFMLWindow {

public:

	SFMLWindow(int width, int height, const std::string& title);
	~SFMLWindow();

	void loop();
	void setupGUI(std::vector<std::shared_ptr<Player>>& players);

	sf::RenderWindow* getWindow();

	bool black = true;

private:

	int width;
	int height;
	const std::string& title;


	sf::RenderWindow window;
	std::list<std::shared_ptr<IRenderable>> renderables;
};