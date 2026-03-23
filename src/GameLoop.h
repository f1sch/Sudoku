#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class IUpdatable;

class GameLoop
{
public:
	GameLoop(IUpdatable* updatable, bool* running);

	void Run();
private:
	void Input();

	IUpdatable* m_updatable;
	bool& m_running;
	sf::RenderWindow m_window;
};

