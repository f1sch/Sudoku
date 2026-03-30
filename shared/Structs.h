#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <string>

struct SpriteEntry
{
    std::string texturePath;
    sf::Texture texture;
    sf::Sprite  sprite{ texture };
    float x = 0.f, y = 0.f;
    float scaleX = 1.f, scaleY = 1.f;
    int layer = 0;

    bool fromTilemap = false;
    sf::IntRect tileRect = {};
    int tileCol = 0;
    int tileRow = 0;

    SpriteEntry(const std::string& path, float px, float py, sf::IntRect rect = {})
        : texturePath(path), x(px), y(py),
        fromTilemap(rect != sf::IntRect{}), tileRect(rect)
    {
        texture.loadFromFile(path);
        sprite.setTexture(texture, true);
        if (fromTilemap)
            sprite.setTextureRect(tileRect);
        sprite.setPosition({ px, py });
    }

    SpriteEntry(const SpriteEntry&) = delete;
    SpriteEntry& operator=(const SpriteEntry&) = delete;
};

struct TilemapAsset
{
    std::string path;
    sf::Texture texture;
    int tileW = 32;
    int tileH = 32;
    int cols = 0;
    int rows = 0;

    bool load(const std::string& filePath, int tw, int th)
    {
        if (!texture.loadFromFile(filePath)) return false;
        path = filePath;
        tileW = tw;
        tileH = th;
        cols = static_cast<int>(texture.getSize().x) / tw;
        rows = static_cast<int>(texture.getSize().y) / th;
        return true;
    }

    sf::IntRect rectAt(int col, int row) const
    {
        return sf::IntRect({ col * tileW, row * tileH }, { tileW, tileH });
    }
};