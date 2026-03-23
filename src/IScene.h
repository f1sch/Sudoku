#pragma once
#include "IUpdatable.h"

#include <SFML/Graphics/Drawable.hpp>

#include <vector>

class IScene : public IUpdatable
{
public:
	virtual void Render(std::vector<const sf::Drawable*>& queue) = 0;
	virtual ~IScene() = default;
};