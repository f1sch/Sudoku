#include "GameScene.h"

#include "AssetManager.h"
#include "GridSystem.h"
#include "../shared/Structs.h"

#include <SFML/Graphics/Drawable.hpp>
#include <nlohmann/json.hpp>

#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <nlohmann/json_fwd.hpp>

GameScene::GameScene(AssetManager& am, GridSystem& gs)
	: m_BoardMockUpSprite(am.Get(AssetManager::TextureID::Board)),
	m_cellSprite(am.Get(AssetManager::TextureID::CellEmpty))
{
	LoadSceneFrom("scene.json");
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
	for (const auto& sprite : m_sprites)
	{
		queue.push_back(&sprite->sprite);
	}
}

void GameScene::LoadSceneFrom(const std::string& file)
{
	std::ifstream in(file);
	nlohmann::json scene;
	in >> scene;

	for (auto& s : scene["sprites"])
	{
		auto sprite = std::make_unique<SpriteEntry>(
			s["texturePath"],
			s["x"],
			s["y"]
		);

		sprite->scaleX = s["scaleX"];
		sprite->scaleY = s["scaleY"];
		sprite->layer = s["layer"];
		sprite->sprite.setScale({ sprite->scaleX, sprite->scaleY });

		m_sprites.push_back(std::move(sprite));
	}
}
