#include "SceneManager.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>
#include <utility>
#include <vector>

SceneManager::SceneManager(AssetManager& am, GridSystem& gs)
{
}

void SceneManager::requestSceneChange(std::unique_ptr<IScene> next)
{
	m_nextScene = std::move(next);
}

void SceneManager::onKeyPressed(sf::Keyboard::Key key)
{
	m_currentScene->onKeyPressed(key);
}

void SceneManager::update()
{
	if (m_nextScene)
	{
		if (m_currentScene) m_currentScene->onExit();
		m_currentScene = std::move(m_nextScene);
		m_currentScene->onEnter();
	}

	if (m_currentScene)
		m_currentScene->update();
}

void SceneManager::render()
{
}

void SceneManager::render(std::vector<const sf::Drawable*>& queue)
{
	// Fill RenderQueue with Objects
	if (m_currentScene)
		m_currentScene->render(queue);
}

void SceneManager::processEvent(const sf::Event& event)
{
	if (m_currentScene)
		m_currentScene->processEvent(event);
}
