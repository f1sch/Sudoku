#include "GameScene.h"

#include "AssetManager.h"
#include "Board.h"
#include "GridSystem.h"

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <algorithm>
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

GameScene::GameScene(AssetManager& am, GridSystem& gs)
	: m_gridSystem(gs)
{
	LoadSceneFrom("scene.json");
	m_board = std::make_unique<Board>();
	
	m_numbersTex = &am.Get(AssetManager::TextureID::Number);
	RebuildNumberSprites();
	
	m_cursor.setPosition(gs.tileToWorld(m_cursorCol, m_cursorRow));
	m_cursor.setSize(sf::Vector2f(TEX_WIDTH, TEX_WIDTH));
	m_cursor.setFillColor(sf::Color::Transparent);
	m_cursor.setOutlineColor(sf::Color::Green);
	m_cursor.setOutlineThickness(2.f);
}

void GameScene::Update()
{
	m_cursor.setPosition(m_gridSystem.tileToWorld(m_cursorCol, m_cursorRow));
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
	queue.push_back(&m_cursor);
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

void GameScene::OnKeyPressed(sf::Keyboard::Key key)
{
	static const std::map<sf::Keyboard::Key, int> keyToNumber = {
		{ sf::Keyboard::Key::Delete, 0},
		{ sf::Keyboard::Key::Num1, 1},
		{ sf::Keyboard::Key::Num2, 2},
		{ sf::Keyboard::Key::Num3, 3},
		{ sf::Keyboard::Key::Num4, 4},
		{ sf::Keyboard::Key::Num5, 5},
		{ sf::Keyboard::Key::Num6, 6},
		{ sf::Keyboard::Key::Num7, 7},
		{ sf::Keyboard::Key::Num8, 8},
		{ sf::Keyboard::Key::Num9, 9},
		{ sf::Keyboard::Key::Numpad1, 1},
		{ sf::Keyboard::Key::Numpad2, 2},
		{ sf::Keyboard::Key::Numpad3, 3},
		{ sf::Keyboard::Key::Numpad4, 4},
		{ sf::Keyboard::Key::Numpad5, 5},
		{ sf::Keyboard::Key::Numpad6, 6},
		{ sf::Keyboard::Key::Numpad7, 7},
		{ sf::Keyboard::Key::Numpad8, 8},
		{ sf::Keyboard::Key::Numpad9, 9},
	};

	if (auto it = keyToNumber.find(key); it != keyToNumber.end())
	{
		m_board->SetCell(m_cursorRow, m_cursorCol, it->second);
		RebuildNumberSprites();
	}
	if (key == sf::Keyboard::Key::Up)
		m_cursorRow = std::clamp(m_cursorRow - 1, 0, 8);
	if (key == sf::Keyboard::Key::Down)
		m_cursorRow = std::clamp(m_cursorRow + 1, 0, 8);
	if (key == sf::Keyboard::Key::Left)
		m_cursorCol = std::clamp(m_cursorCol - 1, 0, 8);
	if (key == sf::Keyboard::Key::Right)
		m_cursorCol = std::clamp(m_cursorCol + 1, 0, 8);

	if (m_board->IsSolved())
		std::cout << "Game won!" << std::endl;
}

void GameScene::RebuildNumberSprites()
{
	m_numbersInCells.clear();
	for (int r = 0; r < 9; ++r)
	{
		for (int c = 0; c < 9; ++c)
		{
			auto number = m_board->GetCell(r, c).number;
			if (number == 0) continue;
			sf::Sprite s(*m_numbersTex);
			int index = number - 1;
			int texCol = index % 3;
			int texRow = index / 3;
			s.setTextureRect(sf::IntRect({ texCol * 32, texRow * 32 }, { 32,32 }));
			s.setPosition(m_gridSystem.tileToWorld(c, r));
			if (m_board->GetCell(r, c).canEdit)
				s.setColor(sf::Color::Blue);
			else
				s.setColor(sf::Color::Black);
			m_numbersInCells.push_back(s);
		}
	}
}
