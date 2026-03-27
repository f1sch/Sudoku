#pragma once
#include "../shared/Structs.h"
#include "IScene.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <memory>
#include <string>
#include <vector>

class AssetManager;
class GridSystem;

class GameScene : public IScene
{
public:
	GameScene(AssetManager& am, GridSystem& gs);

	void Update() override;
	void Render() override;
	void Render(std::vector<const sf::Drawable*>& queue) override;
	void LoadSceneFrom(const std::string& file) override;
private:
	sf::Sprite m_BoardMockUpSprite;
	sf::Sprite m_cellSprite;
	std::vector<sf::Sprite> m_cells;
	std::vector<std::unique_ptr<SpriteEntry>> m_sprites;
};

