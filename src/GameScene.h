#pragma once
#include "IScene.h"

#include "Board.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>
#include <string>
#include <vector>

class AssetManager;
class GridSystem;

struct LoadedSprite
{
	sf::Texture texture;
	sf::Sprite sprite{ texture };
	int layer = 0;

	LoadedSprite(const LoadedSprite&) = delete;
	LoadedSprite& operator=(const LoadedSprite&) = delete;
	LoadedSprite() = default;
};

class GameScene : public IScene
{
public:
	GameScene(AssetManager& am, GridSystem& gs);

	void Update() override;
	void Render() override;
	void Render(std::vector<const sf::Drawable*>& queue) override;
	void ProcessEvent(const sf::Event& event) override;
	void LoadSceneFrom(const std::string& file) override;
	void OnKeyPressed(sf::Keyboard::Key key) override;
private:
	void RebuildNumberSprites();
	GridSystem& m_gridSystem;
	std::vector<std::unique_ptr<LoadedSprite>> m_sprites;
	std::unique_ptr<Board> m_board;
	std::vector<sf::Sprite> m_numbersInCells;
	const sf::Texture* m_numbersTex = nullptr;
	
	sf::RectangleShape m_cursor;
	int m_cursorRow = 0;
	int m_cursorCol = 0;
};

