#pragma once
#include "IInputListener.h"

#include <SFML/Window/Event.hpp>

#include <vector>

class InputSystem
{
public:
	void AddListener(IInputListener* listener);
	void ProcessEvent(const sf::Event& event);

private:
	std::vector<IInputListener*> m_listeners;
};