#pragma once
#include <SFML/Graphics/Texture.hpp>

#include <map>
#include <string>

class AssetManager
{
public:
	enum class TextureID { CellEmpty, Board };

	AssetManager();

	void Load(TextureID id, const std::string& filePath);
	const sf::Texture& Get(TextureID id);
private:
	std::map<TextureID, sf::Texture> m_textures;
};