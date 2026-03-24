#include "GameScene.h"

#include "AssetManager.h"
#include "GridSystem.h"

#include <SFML/Graphics/Drawable.hpp>

#include <vector>

GameScene::GameScene(AssetManager& am, GridSystem& gs)
	: m_BoardMockUpSprite(am.Get(AssetManager::TextureID::Board)),
	m_cellSprite(am.Get(AssetManager::TextureID::CellEmpty))
{
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			m_cellSprite.setPosition(gs.GetCellPosition(i, j));
			m_cells.emplace_back(m_cellSprite);
		}
	}

}

void GameScene::Update()
{
}

void GameScene::Render()
{
}

void GameScene::Render(std::vector<const sf::Drawable*>& queue)
{
	// Submit RenderObjects to RenderQueue
	//queue.push_back(&m_BoardMockUpSprite);
	for (const auto& sprite : m_cells)
	{
		queue.push_back(&sprite);
	}
}
