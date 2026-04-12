#include "GameManager.h"

#include "shared/Data.h"

#include "AssetManager.h"
#include "GameLoop.h"
#include "GridSystem.h"
#include "InputSystem.h"
#include "Renderer.h"
#include "SceneManager.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>

#include <memory>

GameManager::GameManager()
	: m_running(true)
{
	m_renderer = std::make_unique<Renderer>();
	
	m_gameLoop = std::make_unique<GameLoop>(this, &m_running, m_renderer->m_window);
	
	m_assetManager = std::make_unique<AssetManager>();
	m_assetManager->load(AssetManager::TextureID::Number, "assets/textures/numbersWhite.png");

	m_gridSystem = std::make_unique<GridSystem>(
		TILE_SIZE, sf::Vector2f(static_cast<float>(BOARD_LEFT), static_cast<float>(BOARD_TOP)));

	m_sceneManager = std::make_unique<SceneManager>(*m_assetManager, *m_gridSystem);

	m_inputSystem = std::make_unique<InputSystem>();
	m_inputSystem->addListener(this);
	m_inputSystem->addListener(m_sceneManager.get());
}

GameManager::~GameManager() = default;

void GameManager::run()
{
	m_gameLoop->run();
}

void GameManager::onKeyPressed(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::Escape)
	{
		m_running = false;
	}
}

void GameManager::update()
{
	m_sceneManager->update();
}

void GameManager::render()
{
	auto& queue = m_renderer->m_queue;
	m_sceneManager->render(queue);
	m_renderer->flush();
}

void GameManager::processEvent(const sf::Event& event)
{
	m_inputSystem->processEvent(event);

	if (const auto* resized = event.getIf<sf::Event::Resized>())
	{
		m_renderer->handleResize(resized->size.x, resized->size.y);
	}
}
