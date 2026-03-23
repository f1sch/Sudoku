#include "AssetManager.h"

#include <SFML/Graphics/Texture.hpp>

#include <cassert>
#include <iostream>
#include <string>

AssetManager::AssetManager()
{
}

void AssetManager::Load(TextureID id, const std::string& filePath)
{
	sf::Texture tex;
	if (!tex.loadFromFile(filePath))
	{
		std::cerr << "failed to load image" << std::endl;
		return;
	}
	m_textures.insert({ id, tex });
}

const sf::Texture& AssetManager::Get(TextureID id)
{
	assert(m_textures.find(id) != m_textures.end() && "Texture not loaded");

	return m_textures[id];
}
