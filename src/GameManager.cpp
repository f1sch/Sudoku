#include "GameManager.h"

#include "GameLoop.h"
#include "SceneManager.h"

#include <memory>

GameManager::GameManager()
	: m_running(true)
{
	m_gameLoop = std::make_unique<GameLoop>(this, &m_running);
	m_sceneManager = std::make_unique<SceneManager>();
}

void GameManager::Run()
{
	m_gameLoop->Run();
}

void GameManager::OnKeyPressed(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::Escape)
	{
		m_running = false;
	}
}

void GameManager::Update()
{
	m_sceneManager->Update();
}

void GameManager::Render()
{
	m_sceneManager->Render();
	// m_renderer->Flush();
}
