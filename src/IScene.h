#pragma once
#include "IUpdatable.h"

#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <string>

class IScene : public IUpdatable
{
public:
	virtual void Render(std::vector<const sf::Drawable*>& queue) = 0;
	virtual void LoadSceneFrom(const std::string& file) = 0;
	virtual ~IScene() = default;
};