#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class IUpdatable;

class GameLoop
{
public:
	GameLoop(IUpdatable* updatable);

	void Run();
private:
	void Input();

	IUpdatable* m_updatable;

	sf::RenderWindow m_window;
};

