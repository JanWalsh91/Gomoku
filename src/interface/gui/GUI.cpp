#include "GUI.hpp"

const int GUI::NbStoneSoundEffects = 3;

GUI::GUI(std::shared_ptr<Gomoku> gomoku, std::shared_ptr<SFMLWindow> window): _gomoku(gomoku), _window(window), _currentPlayer(0) {
	TextBox::loadFont();

	 for (int i = 1; i <= NbStoneSoundEffects; i++) {
		sf::SoundBuffer buffer;

		if (buffer.loadFromFile("resources/sounds/stone_sound_" + std::to_string(i) + ".wav")) {
			_stoneSoundEffects.push_back(buffer);
		}
	}

	if (!_bgm.openFromFile("resources/sounds/background_music.wav")) {
		std::cerr << "Cannot load background music" << std::endl;
		return ;
	}
	_bgm.play();
	_bgm.setLoop(true);
}

GUI::~GUI() {
}

void GUI::setup() {
	_background = std::make_shared<Background>(sf::Vector2f(790.0f, 790.0f), sf::Vector2f(5.0f, 5.0f), Colors::ForestGreen);
	_background->setBorder(5.0f, Colors::Grey);
	_background->setTexture("resources/textures/background.png");


	_grid = std::make_shared<Grid>(_gomoku->size, 800.0f, 40.0f);

	_grid->hoverCallbacks.push_back([this](sf::Vector2i mousePosition) mutable {
		if (_gomoku->playing) {
			auto circle = sf::CircleShape(_grid->getCellSize() / 2.0f);
			circle.setFillColor(_currentPlayer == 0 ? sf::Color(50, 50, 50, 125) : sf::Color(200, 200, 200, 125));
			circle.setOutlineThickness(1.0f);
			circle.setOutlineColor(sf::Color::Black);
			circle.setPosition(static_cast<float>(mousePosition.y), static_cast<float>(mousePosition.x));
			_window->getWindow()->draw(circle);
		}
	});


	_panel = std::make_shared<Background>(sf::Vector2f(480.0f, 800.0f), sf::Vector2f(800.0f, 0.0f), Colors::LightGrey);
	
	_playersPanel = std::make_shared<Background>(sf::Vector2f(360.0f, 125.0f), sf::Vector2f(820.0f, 20.0f), Colors::LightGrey);
	_playersPanel->setBorder(2.5f, Colors::Grey);


	_playerOneButton = std::make_shared<Button>(sf::Vector2f(100.0f, 100.0f), sf::Vector2f(840.0f, 32.5f), _gomoku->players[0]->isHuman() ? Colors::Human : Colors::AI);
	_playerOneButton->setText(_gomoku->players[0]->isHuman() ? "Human" : "AI");
	_playerOneButton->setFontColor(sf::Color::Black);
	_playerOneButton->setBorder(3.f, sf::Color::Black);
	_playerOneButton->clickCallbacks.push_back([this](sf::Vector2i mousePosition) mutable {
		(void)mousePosition;
		if (_gomoku->players[0]->isHuman()) {
			_playerOneButton->setText("AI");
			_playerOneButton->setBackgroundColor(Colors::AI);
			_gomoku->players[0]->changeType(Player::AI);
		}
		else {
			_playerOneButton->setText("Human");
			_playerOneButton->setBackgroundColor(Colors::Human);
			_gomoku->players[0]->changeType(Player::HUMAN);
		}
	});

	_playerTwoButton = std::make_shared<Button>(sf::Vector2f(100.0f, 100.0f), sf::Vector2f(1060.0f, 32.5f), _gomoku->players[1]->isHuman() ? Colors::Human : Colors::AI);
	_playerTwoButton->setText(_gomoku->players[1]->isHuman() ? "Human" : "AI");
	_playerTwoButton->setFontColor(sf::Color::White);
	_playerTwoButton->setBorder(3.f, sf::Color::White);
	_playerTwoButton->clickCallbacks.push_back([this](sf::Vector2i mousePosition) mutable {
		(void)mousePosition;
		if (_gomoku->players[1]->isHuman()) {
			_playerTwoButton->setText("AI");
			_gomoku->players[1]->changeType(Player::AI);
			_playerTwoButton->setBackgroundColor(Colors::AI);
		}
		else {
			_playerTwoButton->setText("Human");
			_playerTwoButton->setBackgroundColor(Colors::Human);
			_gomoku->players[1]->changeType(Player::HUMAN);
		}
	});

	_vsLabel = std::make_shared<TextBox>(sf::Vector2f(50.0f, 50.0f), sf::Vector2f(975.0f, 60.0f), Colors::LightGrey);
	_vsLabel->setText("vs");
	_vsLabel->setFontColor(sf::Color::Black);

	_capturesPanel = std::make_shared<Background>(sf::Vector2f(360.0f, 80.0f), sf::Vector2f(820.0f, 160.0f), Colors::LightGrey);
	_capturesPanel->setBorder(2.5f, Colors::Grey);

	_captureLabel = std::make_shared<TextBox>(sf::Vector2f(360.0f, 50.0f), sf::Vector2f(820.0f, 160.0f), sf::Color(220, 220, 220, 0));
	_captureLabel->setText("Captures:");
	_captureLabel->setFontColor(sf::Color::Black);

	_playerOneCaptures = std::make_shared<TextBox>(sf::Vector2f(180.0f, 50.0f), sf::Vector2f(800.0f, 190.0f), sf::Color(220, 220, 220, 0));
	_playerOneCaptures->setText("0");
	_playerOneCaptures->setFontColor(sf::Color::Black);

	_playerTwoCaptures = std::make_shared<TextBox>(sf::Vector2f(180.0f, 50.0f), sf::Vector2f(1020.0f, 190.0f), sf::Color(220, 220, 220, 0));
	_playerTwoCaptures->setText("0");
	_playerTwoCaptures->setFontColor(sf::Color::Black);
	
	_playPanel = std::make_shared<Background>(sf::Vector2f(360.0f, 125.0f), sf::Vector2f(820.0f, 260.0f), Colors::LightGrey);
	_playPanel->setBorder(2.5f, Colors::Grey);

	_playButton = std::make_shared<Button>(sf::Vector2f(300.0f, 105.0f), sf::Vector2f(850.0f, 270.0f), Colors::ForestGreen);
	_playButton->setText("START !");
	_playButton->setFontColor(sf::Color::Black);
	_playButton->setBorder(2.5f, sf::Color::Black);

	_playButton->clickCallbacks.push_back([this](sf::Vector2i mousePosition) mutable {
		(void)mousePosition;
		// if (_gomoku->minmax->isRunning()) {
		// 	std::cout << "Pause the game first" << std::endl;
		// 	return;
		// }
		if (_gomoku->playing || _gomoku->getEndState() != -1) {
			_gomoku->reset();
			reset();
			std::cout << "Reset gomoku" << std::endl;
			if (_gomoku->players[0]->isAI()) {
				_playButton->setText("START !");
				std::cout << "set text only start" << std::endl;
			} else {
				std::cout << "set playing ot true" << std::endl;
				_gomoku->playing = true;
			}
		}
		else {
			std::cout << "set text reset and playing to true" << std::endl;
			_gomoku->clearReset();
			_playButton->setText("RESET");
			_gomoku->playing = true;
		}
		std::cout << "isAI? " << _gomoku->players[0]->isAI() << std::endl;
	});

	_currentPlayerPanel = std::make_shared<Background>(sf::Vector2f(360.0f, 150.0f), sf::Vector2f(820.0f, 400.0f), Colors::LightGrey);
	_currentPlayerPanel->setBorder(2.5f, Colors::Grey);
	
	_currentPlayerLabel = std::make_shared<TextBox>(sf::Vector2f(360.0f, 50.0f), sf::Vector2f(820.0f, 400.0f), sf::Color(220, 220, 220, 0));
	_currentPlayerLabel->setText("Current Player: ");
	_currentPlayerLabel->setFontColor(sf::Color::Black);

	_currentPlayerValue = std::make_shared<TextBox>(sf::Vector2f(360.0f, 75.0f), sf::Vector2f(820.0f, 450.0f), sf::Color(220, 220, 220, 0));
	_currentPlayerValue->setText("Black");
	_currentPlayerValue->setFontSize(50);
	_currentPlayerValue->setFontColor(sf::Color::Black);

	_messagePanel = std::make_shared<Background>(sf::Vector2f(360.0f, 150.0f), sf::Vector2f(820.0f, 575.0f), Colors::LightGrey);
	_messagePanel->setBorder(2.5f, Colors::Grey);

	_messageValue = std::make_shared<TextBox>(sf::Vector2f(360.0f, 150.0f), sf::Vector2f(820.0f, 575.0f), sf::Color(220, 220, 220, 0));
	_messageValue->setText("-");
	_messageValue->setFontColor(sf::Color::Black);



	_grid->clickCallbacks.push_back([this](sf::Vector2i mousePosition) mutable {
		if (_gomoku->playing) {
			if (_gomoku->currentPlayer->isAI() || _gomoku->minmax->isRunning()) {
				std::cout << "AI Turn, be patient!" << std::endl;
				return;
			}
			auto pos = _grid->windowToGridCoord(mousePosition);
			if (pos.first != -1 && pos.second != -1 && _gomoku->getValueOnBoard(pos) == -1 && _gomoku->canPlace(pos)) {
				_gomoku->lastMoves[_gomoku->currentPlayer->getIndex()] = pos;
				_gomoku->place(pos.first, pos.second, _gomoku->currentPlayer->getIndex());
				_gomoku->switchPlayer();
				nextTurn();
			}
		}
	});

	_window->addRenderable(_background);
	_window->addRenderable(_panel);
	_window->addRenderable(_playersPanel);
	_window->addRenderable(_grid);

	_window->addRenderable(_playerOneButton);
	_window->addRenderable(_playerTwoButton);
	_window->addRenderable(_vsLabel);

	_window->addRenderable(_capturesPanel);
	_window->addRenderable(_playerOneCaptures);
	_window->addRenderable(_playerTwoCaptures);
	_window->addRenderable(_captureLabel);

	_window->addRenderable(_playPanel);
	_window->addRenderable(_playButton);

	_window->addRenderable(_currentPlayerPanel);
	_window->addRenderable(_currentPlayerLabel);
	_window->addRenderable(_currentPlayerValue);

	_window->addRenderable(_messagePanel);
	_window->addRenderable(_messageValue);
}

void GUI::updateBoard(std::pair<int, int> pos, int value) {

	if (value >= 0) {
		_grid->placeStoneAt(pos, value == 0 ? sf::Color::Black : sf::Color::White);
	} else if (value == -1) {
		_grid->removeStoneAt(pos);
	}

	 _sfx.setBuffer(_stoneSoundEffects[std::rand() % _stoneSoundEffects.size()]);
	 _sfx.play();
}

void GUI::updateCaptures(int playerIndex, int value) {
	if (playerIndex == 0) {
		_playerOneCaptures->setText(std::to_string(value));
	} else {
		_playerTwoCaptures->setText(std::to_string(value));
	}
}


void GUI::nextTurn() {
	if (_gomoku->getEndState() != -1) {
		_gomoku->playing = false;
		if (_gomoku->getEndState() >= 0) {
			_messageValue->setText((_gomoku->getEndState() == 0 ? "Black" : "White") + std::string(" win! ") + std::to_string(_gomoku->turn));
		}
		else {
			_messageValue->setText("DRAW");
		}
	}
	else {
		_currentPlayerValue->setText(_gomoku->currentPlayer->getIndex() == 0 ? "Black" : "White");
		_currentPlayerValue->setFontColor(_gomoku->currentPlayer->getIndex() == 0 ? sf::Color::Black : sf::Color::White);
		if (_gomoku->players[_currentPlayer]->isAI()) {
			std::stringstream ss;
			ss << std::fixed << std::setprecision(2) << Timer::Get("MinmaxRun") / 1000.0;
			_messageValue->setText(std::to_string(_gomoku->getTurn()) + " (" + ss.str() + ")");
		}
		else {
			_messageValue->setText(std::to_string(_gomoku->getTurn()));
		}
	}
	_currentPlayer = _currentPlayer == 0 ? 1 : 0;
	_gomoku->nextTurn();
}

void GUI::reset() {
	_grid->reset();
	_messageValue->setText("-");
	_currentPlayer = 0;
	_currentPlayerValue->setText("Black");
	_playerOneCaptures->setText("0");
	_playerTwoCaptures->setText("0");
}