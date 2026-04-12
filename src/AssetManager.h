#pragma once
#include <SFML/Graphics/Texture.hpp>

#include <map>
#include <string>

class AssetManager
{
public:
	enum class TextureID { CellEmpty, Board, Number };

	AssetManager();

	void load(TextureID id, const std::string& filePath);
	const sf::Texture& findTexture(TextureID id);
private:
	std::map<TextureID, sf::Texture> m_textures;
};