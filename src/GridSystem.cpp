#include "GridSystem.h"

#include <SFML/System/Vector2.hpp>

GridSystem::GridSystem(int tileSize, sf::Vector2f gridOrigin)
{
    m_tileSize = tileSize;
    m_gridOrigin = gridOrigin;
}

sf::Vector2f GridSystem::tileToWorld(int tileX, int tileY) const
{
    return {
        m_gridOrigin.x + tileX * m_tileSize,
        m_gridOrigin.y + tileY * m_tileSize
    };
}

sf::Vector2f GridSystem::worldToTile(sf::Vector2f pos) const
{
    int x = (pos.x - m_gridOrigin.x) / m_tileSize;
    int y = (pos.y - m_gridOrigin.y) / m_tileSize;

    return { static_cast<float>(x), static_cast<float>(y) };
}