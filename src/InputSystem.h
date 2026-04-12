#pragma once
#include "IInputListener.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>

class InputSystem
{
public:
	void AddListener(IInputListener* listener);
	void ProcessEvent(const sf::Event& event);
	sf::Vector2f WindowToVirtual(sf::Vector2i mousePos,
		sf::Vector2f sceneOffset, float scale);

private:
	std::vector<IInputListener*> m_listeners;
};