#pragma once
#include "IScene.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <vector>

class AssetManager;

class GameScene : public IScene
{
public:
	GameScene(AssetManager& am);

	void Update() override;
	void Render() override;
	void Render(std::vector<const sf::Drawable*>& queue) override;
private:
	const sf::Texture testTex;
	sf::Sprite m_testSprite;
};

