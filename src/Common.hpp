#pragma once


#include <iostream>
#include <string>
#include <cmath>
#include <memory>
#include <functional>
#include <list>
#include <iomanip>
#include <chrono>
#include <future>

#include <args.hxx>

#include <SFML/Graphics/Shape.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


#include "Colors.hpp"
#include "AAction.hpp"
#include "ActionUpdateBoard.hpp"
#include "ActionDecrementCapture.hpp"
#include "ActionIncrementCapture.hpp"
#include "ActionSetEndState.hpp"

#include "ARule.hpp"
#include "NoDoubleFreeThree.hpp"
#include "IRenderable.hpp"
#include "Background.hpp"
#include "Grid.hpp"
#include "Button.hpp"
#include "TextBox.hpp"

#include "Player.hpp"
#include "Gomoku.hpp"
#include "SFMLWindow.hpp"
#include "Minmax.hpp"

#include "GUI.hpp"