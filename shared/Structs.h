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

    SpriteEntry(const std::string& path, float px, float py)
        : texturePath(path), x(px), y(py)
    {
        texture.loadFromFile(path);
        sprite.setTexture(texture, true);
        sprite.setPosition({ px, py });
    }

    SpriteEntry(const SpriteEntry&) = delete;
    SpriteEntry& operator=(const SpriteEntry&) = delete;
};