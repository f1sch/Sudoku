#include "GameManager.h"

#include "GameLoop.h"

#include <memory>

GameManager::GameManager()
{
	m_gameLoop = std::make_unique<GameLoop>(this);
}

void GameManager::Run()
{
	m_gameLoop->Run();
}

void GameManager::Update()
{
	// Update Scene
}

void GameManager::Render()
{
	// Call Renderer
}
