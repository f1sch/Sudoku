#include "GameScene.h"

#include "AssetManager.h"
#include "Board.h"
#include "GridSystem.h"

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

#include <algorithm>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
//testing
#include <iostream>

GameScene::GameScene(AssetManager& am, GridSystem& gs)
{
	LoadSceneFrom("scene.json");
	m_board = std::make_unique<Board>();
	// for testing
	for (int i = 0; i < 9; ++i)
	{
		if (i % 3 == 0 && i != 0)
		{
			std::cout << "------+-------+------" << std::endl;
		}
		for (int j = 0; j < 9; ++j)
		{
			if (j % 3 == 0 && j != 0)
			{
				std::cout << "| ";
			}
			std::cout << m_board->GetCell(i, j).number << " ";
		}
		std::cout << std::endl;
	}
	// This only creates the number textures from the number spritesheet.
	// The correct position on the grid has to be set for the 81 tiles
	const sf::Texture& numbers = am.Get(AssetManager::TextureID::Number);
	// Board holds the Cells, these Cells have a member that defines which number they hold.
	// The GameScene pulls all the data from the different Systems and ties it together
	// create RenderObjects
	for (int r = 0; r < 9; ++r)
	{
		for (int c = 0; c < 9; ++c)
		{
			auto number = m_board->GetCell(r, c).number;
			if (number == 0) continue;
			sf::Sprite s(numbers);
			int index = number - 1;
			int texCol = index % 3;
			int texRow = index / 3;
			s.setTextureRect(sf::IntRect({ texCol * 32, texRow * 32 }, { 32,32 }));
			s.setPosition(gs.tileToWorld(c, r));
			m_numbersInCells.push_back(s);
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
	for (const auto& sprite : m_sprites)
	{
		queue.push_back(&sprite->sprite);
	}
	for (const auto& number : m_numbersInCells)
	{
		queue.push_back(&number);
	}
}

void GameScene::ProcessEvent(const sf::Event& event)
{

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
