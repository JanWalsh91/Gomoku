#include <iostream>
#include <memory>

#include "SFMLWindow.hpp"

int main() {
	{
		std::cout << "hello" << std::endl;

		std::shared_ptr<SFMLWindow> window = std::make_shared<SFMLWindow>(1280, 800, "Title");

		window->loop();
	}
	return (0);
}