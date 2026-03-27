#include "SceneManager.h"

#include "GameScene.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>
#include <vector>

SceneManager::SceneManager(AssetManager& am, GridSystem& gs)
{
	m_scene = std::make_unique<GameScene>(am, gs);
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
}

void SceneManager::Render(std::vector<const sf::Drawable*>& queue)
{
	// Fill RenderQueue with Objects
	m_scene->Render(queue);
}