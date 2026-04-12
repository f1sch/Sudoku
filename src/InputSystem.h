#pragma once
#include "IInputListener.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>

class InputSystem
{
public:
	void addListener(IInputListener* listener);
	void processEvent(const sf::Event& event);
	sf::Vector2f windowToVirtual(sf::Vector2i mousePos,
		sf::Vector2f sceneOffset, float scale);

private:
	std::vector<IInputListener*> m_listeners;
};