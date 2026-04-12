#include "InputSystem.h"

#include "IInputListener.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

void InputSystem::AddListener(IInputListener* listener)
{
	m_listeners.push_back(listener);
}

void InputSystem::ProcessEvent(const sf::Event& event)
{
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		for (auto listener : m_listeners)
		{
			listener->OnKeyPressed(keyPressed->code);
		}
	}
}

// Future support for mouse events 
sf::Vector2f InputSystem::WindowToVirtual(sf::Vector2i mousePos, sf::Vector2f sceneOffset, float scale)
{
	return {
		(mousePos.x - sceneOffset.x) / scale,
		(mousePos.y - sceneOffset.y) / scale
	};
}
