#include "GameLoop.h"

#include "IUpdatable.h"

#include <optional>

GameLoop::GameLoop(IUpdatable* updatable)
{
	m_updatable = updatable;
	m_window = sf::RenderWindow(sf::VideoMode({ 200, 200 }), "SFML works!");
}

void GameLoop::Run()
{
	while (m_window.isOpen())
	{
		Input();
		m_updatable->Update();
		m_updatable->Render();
	}
}

void GameLoop::Input()
{
	while (const std::optional event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			m_window.close();
	}
}