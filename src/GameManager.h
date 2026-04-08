#pragma once
#include "IUpdatable.h"
#include "IInputListener.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>

class AssetManager;
class GameLoop;
class Renderer;
class SceneManager;
class GridSystem;
class InputSystem;

class GameManager : public IUpdatable, public IInputListener
{
public:
	GameManager();
	~GameManager();

	void Run();
	void OnKeyPressed(sf::Keyboard::Key key) override;
private:
	void Update() override;
	void Render() override;
	void ProcessEvent(const sf::Event& event) override;

	std::unique_ptr<GameLoop> m_gameLoop;
	std::unique_ptr<SceneManager> m_sceneManager;
	std::unique_ptr<AssetManager> m_assetManager;
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<GridSystem> m_gridSystem;
	std::unique_ptr<InputSystem> m_inputSystem;

	bool m_running;
};

