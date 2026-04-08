#include "InputSystem.h"

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
