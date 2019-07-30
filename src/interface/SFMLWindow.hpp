#pragma once

#include <string>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <memory>
#include <list>
#include <functional>


#include "Gomoku.hpp"

#include "Player.hpp"
#include "IRenderable.hpp"

class SFMLWindow {

public:

	SFMLWindow(int width, int height, const std::string& title);
	~SFMLWindow();

	void loop(std::function<void()> f);
	void setupGUI(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Gomoku> gomoku);

	sf::RenderWindow* getWindow();

	void addRenderable(std::shared_ptr<IRenderable> renderable);

private:

	int width;
	int height;
	const std::string& title;


	sf::RenderWindow window;
	std::list<std::shared_ptr<IRenderable>> renderables;
};