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

//constexpr int TILE_SIZE = 32;

GameManager::GameManager()
	: m_running(true)
{
	m_renderer = std::make_unique<Renderer>();
	
	m_gameLoop = std::make_unique<GameLoop>(this, &m_running, m_renderer->GetWindow());
	
	m_assetManager = std::make_unique<AssetManager>();
	m_assetManager->Load(AssetManager::TextureID::Number, "assets/textures/numbers.png");

	sf::Vector2 size = m_renderer->GetWindow().getSize();
	auto [width, height] = size;
	//m_gridSystem = std::make_unique<GridSystem>(TILE_SIZE, sf::Vector2f(width / 2.f, height / 2.f));
	//m_gridSystem = std::make_unique<GridSystem>(TILE_SIZE, sf::Vector2f(BOARD_RIGHT/2.f, BOARD_BOTTOM/2.f));
	m_gridSystem = std::make_unique<GridSystem>(TILE_SIZE, sf::Vector2f(160.0f, 160.0f));

	m_sceneManager = std::make_unique<SceneManager>(*m_assetManager, *m_gridSystem);

	m_inputSystem = std::make_unique<InputSystem>();
	m_inputSystem->AddListener(this);
	m_inputSystem->AddListener(m_sceneManager.get());
}

GameManager::~GameManager() = default;

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
	auto& queue = m_renderer->GetQueue();
	m_sceneManager->Render(queue);
	m_renderer->Flush();
}

void GameManager::ProcessEvent(const sf::Event& event)
{
	m_inputSystem->ProcessEvent(event);
}
