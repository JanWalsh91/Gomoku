#include "GUI.hpp"

const int GUI::NbStoneSoundEffects = 3;

GUI::GUI(std::shared_ptr<Gomoku> gomoku, std::shared_ptr<SFMLWindow> window): gomoku(gomoku), window(window), _currentPlayer(0) {
	TextBox::loadFont();

	for (int i = 1; i <= NbStoneSoundEffects; i++) {
		sf::SoundBuffer buffer;

		if (buffer.loadFromFile("resources/sounds/stone_sound_" + std::to_string(i) + ".wav")) {
			_stoneSoundEffects.push_back(buffer);
		}
	}
}

void GUI::setup() {
	background = std::make_shared<Background>(790.0f, 790.0f, 5.0f, 5.0f, Colors::ForestGreen);
	background->setBorder(5.0f, Colors::Grey);
	background->setTexture("resources/textures/background.png");


	grid = std::make_shared<Grid>(gomoku->size, 800.0f, 40.0f);

	grid->hoverCallbacks.push_back([this](sf::Vector2i mousePosition) mutable {
		
		//std::cout << "black: " << black << std::endl;

		if (gomoku->playing) {
			auto circle = sf::CircleShape(grid->getCellSize() / 2.0f);
			circle.setFillColor(_currentPlayer == 0 ? sf::Color(50, 50, 50, 125) : sf::Color(200, 200, 200, 125));
			circle.setOutlineThickness(1.0f);
			circle.setOutlineColor(sf::Color::Black);
			circle.setPosition(mousePosition.y, mousePosition.x);
			window->getWindow()->draw(circle);
		}
	});


	panel = std::make_shared<Background>(480.0f, 800.0f, 800.0f, 0.0f, Colors::LightGrey);
	
	playersPanel = std::make_shared<Background>(360.0f, 125.0f, 820.0f, 20.0f, Colors::LightGrey);
	playersPanel->setBorder(2.5f, Colors::Grey);


	playerOneButton = std::make_shared<Button>(100.0f, 100.0f, 840.0f, 32.5f, gomoku->players[0]->isHuman() ? Colors::Human : Colors::AI);
	playerOneButton->setText(gomoku->players[0]->isHuman() ? "Human" : "AI");
	playerOneButton->setFontColor(sf::Color::Black);
	playerOneButton->setBorder(3.f, sf::Color::Black);
	playerOneButton->callbacks.push_back([this](sf::Vector2i mousePosition) mutable {
		if (gomoku->players[0]->isHuman()) {
			playerOneButton->setText("AI");
			playerOneButton->setBackgroundColor(Colors::AI);
			gomoku->players[0]->changeType(Player::AI);
		}
		else {
			playerOneButton->setText("Human");
			playerOneButton->setBackgroundColor(Colors::Human);
			gomoku->players[0]->changeType(Player::HUMAN);
		}
	});

	playerTwoButton = std::make_shared<Button>(100.0f, 100.0f, 1060.0f, 32.5f, gomoku->players[1]->isHuman() ? Colors::Human : Colors::AI);
	playerTwoButton->setText(gomoku->players[1]->isHuman() ? "Human" : "AI");
	playerTwoButton->setFontColor(sf::Color::White);
	playerTwoButton->setBorder(3.f, sf::Color::White);
	playerTwoButton->callbacks.push_back([this](sf::Vector2i mousePosition) mutable {
		if (gomoku->players[1]->isHuman()) {
			playerTwoButton->setText("AI");
			gomoku->players[1]->changeType(Player::AI);
			playerTwoButton->setBackgroundColor(Colors::AI);
		}
		else {
			playerTwoButton->setText("Human");
			playerTwoButton->setBackgroundColor(Colors::Human);
			gomoku->players[1]->changeType(Player::HUMAN);
		}
	});

	vsLabel = std::make_shared<TextBox>(50.0f, 50.0f, 975.0f, 60.0f, Colors::LightGrey);
	vsLabel->setText("vs");
	vsLabel->setFontColor(sf::Color::Black);

	capturesPanel = std::make_shared<Background>(360.0f, 80.0f, 820.0f, 160.0f, Colors::LightGrey);
	capturesPanel->setBorder(2.5f, Colors::Grey);

	captureLabel = std::make_shared<TextBox>(360.0f, 50.0f, 820.0f, 160.0f, sf::Color(220, 220, 220, 0));
	captureLabel->setText("Captures:");
	captureLabel->setFontColor(sf::Color::Black);

	playerOneCaptures = std::make_shared<TextBox>(180.0f, 50.0f, 800.0f, 190.0f, sf::Color(220, 220, 220, 0));
	playerOneCaptures->setText("0");
	playerOneCaptures->setFontColor(sf::Color::Black);

	playerTwoCaptures = std::make_shared<TextBox>(180.0f, 50.0f, 1020.0f, 190.0f, sf::Color(220, 220, 220, 0));
	playerTwoCaptures->setText("0");
	playerTwoCaptures->setFontColor(sf::Color::Black);
	
	playPanel = std::make_shared<Background>(360.0f, 125.0f, 820.0f, 260.0f, Colors::LightGrey);
	playPanel->setBorder(2.5f, Colors::Grey);

	playButton = std::make_shared<Button>(300.0f, 105.0f, 850.0f, 270.0f, Colors::ForestGreen);
	playButton->setText("START !");
	playButton->setFontColor(sf::Color::Black);
	playButton->setBorder(2.5f, sf::Color::Black);

	playButton->callbacks.push_back([this](sf::Vector2i mousePosition) mutable {
		if (gomoku->minmax->isRunning()) {
			std::cout << "Pause the game first" << std::endl;
			return;
		}
		if (gomoku->playing || gomoku->endState != -1) {
			playButton->setText("START !");
			gomoku->playing = false;
			gomoku->reset();
			this->reset();
		}
		else {
			playButton->setText("RESET");
			gomoku->playing = true;
		}
	});

	currentPlayerPanel = std::make_shared<Background>(360.0f, 150.0f, 820.0f, 400.0f, Colors::LightGrey);
	currentPlayerPanel->setBorder(2.5f, Colors::Grey);
	
	currentPlayerLabel = std::make_shared<TextBox>(360.0f, 50.0f, 820.0f, 400.0f, sf::Color(220, 220, 220, 0));
	currentPlayerLabel->setText("Current Player: ");
	currentPlayerLabel->setFontColor(sf::Color::Black);

	currentPlayerValue = std::make_shared<TextBox>(360.0f, 75.0f, 820.0f, 450.0f, sf::Color(220, 220, 220, 0));
	currentPlayerValue->setText("Black");
	currentPlayerValue->setFontSize(50);
	currentPlayerValue->setFontColor(sf::Color::Black);

	messagePanel = std::make_shared<Background>(360.0f, 150.0f, 820.0f, 575.0f, Colors::LightGrey);
	messagePanel->setBorder(2.5f, Colors::Grey);

	messageValue = std::make_shared<TextBox>(360.0f, 150.0f, 820.0f, 575.0f, sf::Color(220, 220, 220, 0));
	messageValue->setText("-");
	messageValue->setFontColor(sf::Color::Black);



	grid->callbacks.push_back([this](sf::Vector2i mousePosition) mutable {
		if (gomoku->playing) {
			if (gomoku->minmax->isRunning()) {
				std::cout << "AI Turn, be patient" << std::endl;
				return;
			}
			auto pos = grid->windowToGridCoord(mousePosition);
			if (!place(pos.first, pos.second)) {
				return ;
			}
			gomoku->lastMoves[gomoku->currentPlayer->getIndex()] = pos;
			gomoku->place(pos.first, pos.second, gomoku->currentPlayer->getIndex());
			gomoku->switchPlayer();
			this->nextTurn();
			gomoku->nextTurn();
		}
	});

	window->addRenderable(background);
	window->addRenderable(panel);
	window->addRenderable(playersPanel);
	window->addRenderable(grid);

	window->addRenderable(playerOneButton);
	window->addRenderable(playerTwoButton);
	window->addRenderable(vsLabel);

	window->addRenderable(capturesPanel);
	window->addRenderable(playerOneCaptures);
	window->addRenderable(playerTwoCaptures);
	window->addRenderable(captureLabel);

	window->addRenderable(playPanel);
	window->addRenderable(playButton);

	window->addRenderable(currentPlayerPanel);
	window->addRenderable(currentPlayerLabel);
	window->addRenderable(currentPlayerValue);

	window->addRenderable(messagePanel);
	window->addRenderable(messageValue);
}

bool GUI::place(int xPos, int yPos) {
	bool canPlace = gomoku->canPlace(std::make_pair(xPos, yPos));
	if (canPlace) {
		canPlace = grid->placeStoneAt(std::make_pair(xPos, yPos), gomoku->currentPlayer->getIndex() == 0 ? sf::Color::Black : sf::Color::White);
	}
	if (canPlace) {
		_sfx.setBuffer(_stoneSoundEffects[std::rand() % _stoneSoundEffects.size()]);
		_sfx.play();
	}
	return canPlace;
}

void GUI::nextTurn() {
	if (gomoku->endState != -1) {
		gomoku->playing = false;
		if (gomoku->endState >= 0) {
			messageValue->setText((gomoku->endState == 0 ? "Black" : "White") + std::string(" win! ") + std::to_string(gomoku->turn));
		}
		else {
			messageValue->setText("DRAW");
		}
	}
	else {
		currentPlayerValue->setText(gomoku->currentPlayer->getIndex() == 0 ? "Black" : "White");
		currentPlayerValue->setFontColor(gomoku->currentPlayer->getIndex() == 0 ? sf::Color::Black : sf::Color::White);
		messageValue->setText(std::to_string(gomoku->getTurn()));
	}
	_currentPlayer = _currentPlayer == 0 ? 1 : 0;
}

void GUI::reset() {
	grid->reset();
	messageValue->setText("-");
}