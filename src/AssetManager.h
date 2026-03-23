#pragma once
#include <SFML/Graphics/Texture.hpp>

#include <map>
#include <string>

class AssetManager
{
public:
	// Cell2, Cell3, Cell4, Cell5, Cell6, Cell7, Cell8, Cell9
	enum class TextureID { CellEmpty, Cell1 };

	AssetManager();

	void Load(TextureID id, const std::string& filePath);
	const sf::Texture& Get(TextureID id);
private:
	std::map<TextureID, sf::Texture> m_textures;
};