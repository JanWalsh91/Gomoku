#pragma once

#include "Common.hpp"

class SFMLWindow;
class Background;
class Grid;
class Button;
class TextBox;

class GUI {

public:
	GUI(std::shared_ptr<Gomoku> gomoku, std::shared_ptr<SFMLWindow> window);
	~GUI();

	void setup();

	void updateBoard(std::pair<int, int> pos, int value);
	void updateCaptures(int pos, int value);
	void updateMessage(std::string message);
	void nextTurn();
	void reset();
private:

	std::shared_ptr<Background> _background;
	std::shared_ptr<Grid> _grid;
	std::shared_ptr<Background> _panel;
	std::shared_ptr<Background> _playersPanel;

	std::shared_ptr<Button> _playerOneButton;
	std::shared_ptr<Button> _playerTwoButton;

	std::shared_ptr<TextBox> _vsLabel;
	std::shared_ptr<Background> _capturesPanel;
	std::shared_ptr<TextBox> _captureLabel;
	std::shared_ptr<TextBox> _playerOneCaptures;
	std::shared_ptr<TextBox> _playerTwoCaptures;

	std::shared_ptr<Background> _playPanel;
	std::shared_ptr<Button> _playButton;

	std::shared_ptr<Background> _currentPlayerPanel;
	std::shared_ptr<TextBox> _currentPlayerLabel;
	std::shared_ptr<TextBox> _currentPlayerValue;

	std::shared_ptr<Background> _messagePanel;
	std::shared_ptr<TextBox> _messageValue;


	std::shared_ptr<Gomoku> _gomoku;
	std::shared_ptr<SFMLWindow> _window;

	int _currentPlayer;
	std::vector<sf::SoundBuffer> _stoneSoundEffects;
	sf::Sound _sfx;
	sf::Music _bgm;
	
	static const int NbStoneSoundEffects;



};