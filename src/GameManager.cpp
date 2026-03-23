#include "GameManager.h"

#include "AssetManager.h"
#include "GameLoop.h"
#include "Renderer.h"
#include "SceneManager.h"

#include <SFML/Window/Keyboard.hpp>

#include <memory>

GameManager::GameManager()
	: m_running(true)
{
	m_renderer = std::make_unique<Renderer>();
	
	m_gameLoop = std::make_unique<GameLoop>(this, &m_running, m_renderer->GetWindow());
	
	m_assetManager = std::make_unique<AssetManager>();
	m_assetManager->Load(AssetManager::TextureID::Cell1, "D:\\Projects\\cpp\\Sudoku\\doc\\SudokuMockUp.png");
	
	m_sceneManager = std::make_unique<SceneManager>(*m_assetManager);
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
