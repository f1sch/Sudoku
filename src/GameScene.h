#pragma once
#include "IScene.h"

#include "Board.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Font.hpp>

#include <memory>
#include <string>
#include <vector>
#include <variant>
#include "SceneManager.h"

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

enum class RenderLayer
{
	Background = 0,
	Board = 1,
	UI = 2,
	Overlay = 3
};

struct RenderObject
{
	std::variant<sf::Text, sf::RectangleShape> obj;
	RenderLayer layer;
};

class GameScene : public IScene
{
public:
	GameScene(AssetManager& am, GridSystem& gs, SceneManager& sm);

	void update() override;
	void render() override;
	void render(std::vector<const sf::Drawable*>& queue) override;
	void processEvent(const sf::Event& event) override;
	void loadSceneFrom(const std::string& file) override;
	void onKeyPressed(sf::Keyboard::Key key) override;

private:
	void rebuildNumberSprites();
	void pushSolvedOverlay();

private:
	AssetManager& m_assetManager;
	GridSystem& m_gridSystem;
	SceneManager& m_sceneManager;

	std::vector<std::unique_ptr<LoadedSprite>> m_sprites;
	std::unique_ptr<Board> m_board;
	std::vector<sf::Sprite> m_numbersInCells;
	const sf::Texture* m_numbersTex = nullptr;
	
	std::vector<RenderObject> m_overlayObjects;
	sf::Font m_font;

	sf::RectangleShape m_cursor;
	int m_cursorRow = 0;
	int m_cursorCol = 0;
};

