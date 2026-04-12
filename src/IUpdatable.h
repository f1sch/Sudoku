#pragma once
#include <SFML/Window/Event.hpp>

class IUpdatable
{
public:
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void processEvent(const sf::Event& event) = 0;
	virtual ~IUpdatable() = default;
};