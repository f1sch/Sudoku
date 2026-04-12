#pragma once
#include "IInputListener.h"
#include "IScene.h"
#include "IUpdatable.h"

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

	void requestSceneChange(std::unique_ptr<IScene> next);

	void onKeyPressed(sf::Keyboard::Key key) override;
	void update() override;
	void render() override;
	void render(std::vector<const sf::Drawable*>& queue);
	void processEvent(const sf::Event& event) override;
private:
	std::unique_ptr<IScene> m_currentScene;
	std::unique_ptr<IScene> m_nextScene;
};

