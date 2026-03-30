#include "GameScene.h"

#include "AssetManager.h"
#include "GridSystem.h"

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <algorithm>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

GameScene::GameScene(AssetManager& am, GridSystem& gs)
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

	m_sprites.clear();
	if (!scene.contains("sprites")) return;

	for (const auto& s : scene["sprites"])
	{
		auto ls = std::make_unique<LoadedSprite>();

		std::string path = s["texturePath"].get<std::string>();
		if (!ls->texture.loadFromFile(path))
			continue;

		ls->sprite.setTexture(ls->texture, true);

		bool fromTilemap = s.value("fromTilemap", false);
		if (fromTilemap && s.contains("textureRect"))
		{
			const auto& r = s["textureRect"];
			ls->sprite.setTextureRect(sf::IntRect(
				{ r["left"].get<int>(), r["top"].get<int>() },
				{ r["width"].get<int>(), r["height"].get<int>() }
			));
		}

		float x = s.value("x", 0.f);
		float y = s.value("y", 0.f);
		float scaleX = s.value("scaleX", 1.f);
		float scaleY = s.value("scaleY", 1.f);

		ls->sprite.setPosition({ x, y });
		ls->sprite.setScale({ scaleX, scaleY });
		ls->layer = s.value("layer", 0);
		
		m_sprites.push_back(std::move(ls));
	}
	std::sort(m_sprites.begin(), m_sprites.end(), [](const auto& a, const auto& b) 
		{ return a->layer < b->layer; }
	);
}
