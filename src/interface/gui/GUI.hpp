#pragma once

#include "IRenderable.hpp"
#include "Background.hpp"
#include "Grid.hpp"
#include "Button.hpp"
#include "TextBox.hpp"
#include "Colors.hpp"

#include "Gomoku.hpp"
#include "SFMLWindow.hpp"

class GUI {

public:
	GUI(std::shared_ptr<Gomoku> gomoku, std::shared_ptr<SFMLWindow> window);

	void setup();

	void place(int xPos, int yPos);
	void nextTurn();
	void reset();
private:

	std::shared_ptr<Background> background;
	std::shared_ptr<Grid> grid;
	std::shared_ptr<Background> panel;
	std::shared_ptr<Background> playersPanel;

	std::shared_ptr<Button> playerOneButton;
	std::shared_ptr<Button> playerTwoButton;

	std::shared_ptr<TextBox> vsLabel;
	std::shared_ptr<Background> capturesPanel;
	std::shared_ptr<TextBox> captureLabel;
	std::shared_ptr<TextBox> playerOneCaptures;
	std::shared_ptr<TextBox> playerTwoCaptures;

	std::shared_ptr<Background> playPanel;
	std::shared_ptr<Button> playButton;

	std::shared_ptr<Background> currentPlayerPanel;
	std::shared_ptr<TextBox> currentPlayerLabel;
	std::shared_ptr<TextBox> currentPlayerValue;

	std::shared_ptr<Background> messagePanel;
	std::shared_ptr<TextBox> messageValue;


	std::shared_ptr<Gomoku> gomoku;
	std::shared_ptr<SFMLWindow> window;
};