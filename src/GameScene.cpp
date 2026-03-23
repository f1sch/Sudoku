#include "GameScene.h"

#include "AssetManager.h"

#include <SFML/Graphics/Drawable.hpp>

#include <vector>

GameScene::GameScene(AssetManager& am)
	: m_testSprite(am.Get(AssetManager::TextureID::Board))
{
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
	queue.push_back(&m_testSprite);
}
