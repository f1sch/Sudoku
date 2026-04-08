#pragma once
#include <SFML/Window/Event.hpp>

class IUpdatable
{
public:
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void ProcessEvent(const sf::Event& event) = 0;
	virtual ~IUpdatable() = default;
};