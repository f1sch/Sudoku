#pragma once
#include "IUpdatable.h"
#include "IInputListener.h"

#include "GameLoop.h"
#include "SceneManager.h"

#include <memory>

class GameManager : public IUpdatable, public IInputListener
{
public:
	GameManager();

	void Run();
	void OnKeyPressed(sf::Keyboard::Key key) override;
private:
	void Update() override;
	void Render() override;

	std::unique_ptr<GameLoop> m_gameLoop;
	std::unique_ptr<SceneManager> m_sceneManager;

	bool m_running;
};

