#include "SceneManager.h"
#include "GameScene.h"

#include <memory>

SceneManager::SceneManager()
{
	m_scene = std::make_unique<GameScene>();
}

void SceneManager::OnKeyPressed(sf::Keyboard::Key key)
{
}

void SceneManager::Update()
{
	m_scene->Update();
}

void SceneManager::Render()
{
	// Fill RenderQueue with Objects
	m_scene->Render();
}
