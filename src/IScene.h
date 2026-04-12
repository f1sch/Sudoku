#pragma once
#include "IInputListener.h"
#include "IUpdatable.h"

#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <string>

class IScene : public IUpdatable, public IInputListener
{
public:
	virtual ~IScene() = default;

	virtual void onEnter() {}
	virtual void onExit() {}

	virtual void render(std::vector<const sf::Drawable*>& queue) = 0;
	virtual void loadSceneFrom(const std::string& file) {};
};