#include "GameLoop.h"

#include "IUpdatable.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

GameLoop::GameLoop(IUpdatable* updatable, bool* running, sf::RenderWindow& window)
	: m_updatable(updatable), m_running(*running), m_window(window)
{
}

void GameLoop::run()
{
	while (m_window.isOpen() && m_running)
	{
		input();
		m_updatable->update();
		m_updatable->render();
	}
}

void GameLoop::input()
{
	while (const std::optional event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			m_window.close();
		else
			m_updatable->processEvent(event.value());
	}
}