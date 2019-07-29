#include "SFMLWindow.hpp"

#include <iostream>

SFMLWindow::SFMLWindow(int width, int height, const std::string& title): width(width), height(height), title(title) {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
	this->window.create(sf::VideoMode(width, height), title, sf::Style::Close, settings);
	TextBox::loadFont();
}

SFMLWindow::~SFMLWindow() {

}

void SFMLWindow::setupGUI(std::vector<std::shared_ptr<Player>>& players) {
	auto background = std::make_shared<Background>(790.0f, 790.0f, 5.0f, 5.0f, Colors::ForestGreen);
	background->setBorder(5.0f, Colors::Grey);


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


	auto panel = std::make_shared<Background>(480.0f, 800.0f, 800.0f, 0.0f, Colors::LightGrey);
	
	auto playersPanel = std::make_shared<Background>(360.0f, 125.0f, 820.0f, 20.0f, Colors::LightGrey);
	playersPanel->setBorder(2.5f, Colors::Grey);


	auto playerOneButton = std::make_shared<Button>(100.0f, 100.0f, 840.0f, 32.5f, Colors::Human);
	playerOneButton->setText(players[0]->isHuman() ? "Human" : "AI");
	playerOneButton->setFontColor(sf::Color::Black);
	playerOneButton->setBorder(3.f, sf::Color::Black);
	playerOneButton->callbacks.push_back([players, playerOneButton](sf::Vector2i mousePosition) mutable {
		if (players[0]->isHuman()) {
			playerOneButton->setText("AI");
			players[0]->changeType(Player::AI);
		}
		else {
			playerOneButton->setText("Human");
			players[0]->changeType(Player::HUMAN);
		}
	});

	auto playerTwoButton = std::make_shared<Button>(100.0f, 100.0f, 1060.0f, 32.5f, Colors::AI);
	playerTwoButton->setText(players[1]->isHuman() ? "Human" : "AI");
	playerTwoButton->setFontColor(sf::Color::White);
	playerTwoButton->setBorder(3.f, sf::Color::White);
	playerTwoButton->callbacks.push_back([players, playerTwoButton](sf::Vector2i mousePosition) mutable {
		if (players[1]->isHuman()) {
			playerTwoButton->setText("AI");
			players[1]->changeType(Player::AI);
		}
		else {
			playerTwoButton->setText("Human");
			players[1]->changeType(Player::HUMAN);
		}
	});

	auto vsLabel = std::make_shared<TextBox>(50.0f, 50.0f, 975.0f, 60.0f, Colors::LightGrey);
	vsLabel->setText("vs");
	vsLabel->setFontColor(sf::Color::Black);

	auto capturesPanel = std::make_shared<Background>(360.0f, 80.0f, 820.0f, 160.0f, Colors::LightGrey);
	capturesPanel->setBorder(2.5f, Colors::Grey);

	auto captureLabel = std::make_shared<TextBox>(360.0f, 50.0f, 820.0f, 160.0f, sf::Color(220, 220, 220, 0));
	captureLabel->setText("Captures:");
	captureLabel->setFontColor(sf::Color::Black);

	auto playerOneCaptures = std::make_shared<TextBox>(180.0f, 50.0f, 800.0f, 190.0f, sf::Color(220, 220, 220, 0));
	playerOneCaptures->setText("0");
	playerOneCaptures->setFontColor(sf::Color::Black);

	auto playerTwoCaptures = std::make_shared<TextBox>(180.0f, 50.0f, 1020.0f, 190.0f, sf::Color(220, 220, 220, 0));
	playerTwoCaptures->setText("0");
	playerTwoCaptures->setFontColor(sf::Color::Black);
	
	auto playPanel = std::make_shared<Background>(360.0f, 125.0f, 820.0f, 260.0f, Colors::LightGrey);
	playPanel->setBorder(2.5f, Colors::Grey);

	auto playButton = std::make_shared<Button>(300.0f, 105.0f, 850.0f, 270.0f, Colors::ForestGreen);
	playButton->setText("START !");
	playButton->setFontColor(sf::Color::Black);
	playButton->setBorder(2.5f, sf::Color::Black);

	playButton->callbacks.push_back([playButton](sf::Vector2i mousePosition) mutable {
		playButton->setText("RESET");
	});

	auto currentPlayerPanel = std::make_shared<Background>(360.0f, 150.0f, 820.0f, 400.0f, Colors::LightGrey);
	currentPlayerPanel->setBorder(2.5f, Colors::Grey);
	
	auto currentPlayerLabel = std::make_shared<TextBox>(360.0f, 50.0f, 820.0f, 400.0f, sf::Color(220, 220, 220, 0));
	currentPlayerLabel->setText("Current Player: ");
	currentPlayerLabel->setFontColor(sf::Color::Black);

	auto currentPlayerValue = std::make_shared<TextBox>(360.0f, 75.0f, 820.0f, 450.0f, sf::Color(220, 220, 220, 0));
	currentPlayerValue->setText("Black");
	currentPlayerValue->setFontSize(50);
	currentPlayerValue->setFontColor(sf::Color::Black);

	auto messagePanel = std::make_shared<Background>(360.0f, 150.0f, 820.0f, 575.0f, Colors::LightGrey);
	messagePanel->setBorder(2.5f, Colors::Grey);

	auto messageValue = std::make_shared<TextBox>(360.0f, 150.0f, 820.0f, 575.0f, sf::Color(220, 220, 220, 0));
	messageValue->setText("-");
	messageValue->setFontColor(sf::Color::Black);


	this->renderables.push_back(background);
	this->renderables.push_back(panel);
	this->renderables.push_back(playersPanel);
	this->renderables.push_back(grid);

	this->renderables.push_back(playerOneButton);
	this->renderables.push_back(playerTwoButton);
	this->renderables.push_back(vsLabel);

	this->renderables.push_back(capturesPanel);
	this->renderables.push_back(playerOneCaptures);
	this->renderables.push_back(playerTwoCaptures);
	this->renderables.push_back(captureLabel);

	this->renderables.push_back(playPanel);
	this->renderables.push_back(playButton);

	this->renderables.push_back(currentPlayerPanel);
	this->renderables.push_back(currentPlayerLabel);
	this->renderables.push_back(currentPlayerValue);

	this->renderables.push_back(messagePanel);
	this->renderables.push_back(messageValue);


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