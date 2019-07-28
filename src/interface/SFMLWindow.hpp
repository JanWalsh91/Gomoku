#pragma once

#include <string>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <memory>
#include <list>

#include "IRenderable.hpp"
#include "Background.hpp"
#include "Grid.hpp"

class SFMLWindow {

public:

	SFMLWindow(int width, int height, const std::string& title);
	~SFMLWindow();

	void loop();

private:

	int width;
	int height;
	const std::string& title;

	sf::RenderWindow window;
	std::list<std::shared_ptr<IRenderable>> renderables;
};