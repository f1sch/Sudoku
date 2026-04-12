#include "SceneManager.h"

#include "GameScene.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>
#include <vector>

SceneManager::SceneManager(AssetManager& am, GridSystem& gs)
{
	m_scene = std::make_unique<GameScene>(am, gs);
}

void SceneManager::onKeyPressed(sf::Keyboard::Key key)
{
	m_scene->onKeyPressed(key);
}

void SceneManager::update()
{
	m_scene->update();
}

void SceneManager::render()
{
}

void SceneManager::render(std::vector<const sf::Drawable*>& queue)
{
	// Fill RenderQueue with Objects
	m_scene->render(queue);
}

void SceneManager::processEvent(const sf::Event& event)
{
	m_scene->processEvent(event);
}
