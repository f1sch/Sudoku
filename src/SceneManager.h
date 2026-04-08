#pragma once
#include "IInputListener.h"
#include "IUpdatable.h"
#include "IScene.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>
#include <vector>

class AssetManager;
class GridSystem;

class SceneManager : public IInputListener, public IUpdatable
{
public:
	SceneManager(AssetManager& am, GridSystem& gs);

	void OnKeyPressed(sf::Keyboard::Key key) override;
	void Update() override;
	void Render() override;
	void Render(std::vector<const sf::Drawable*>& queue);
	void ProcessEvent(const sf::Event& event) override;
private:
	std::unique_ptr<IScene> m_scene;
};

