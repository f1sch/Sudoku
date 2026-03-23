#pragma once
#include <SFML/Window/Keyboard.hpp>

class IInputListener
{
public:
	virtual void OnKeyPressed(sf::Keyboard::Key key) = 0;
	virtual ~IInputListener() = default;
};