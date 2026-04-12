#include "GridSystem.h"

#include <SFML/System/Vector2.hpp>

GridSystem::GridSystem(unsigned tileSize, sf::Vector2f gridOrigin)
{
    m_tileSize = tileSize;
    m_gridOrigin = gridOrigin;
}

sf::Vector2f GridSystem::tileToWorld(int col, int row) const
{
    return {
        m_gridOrigin.x + col * static_cast<float>(m_tileSize),
        m_gridOrigin.y + row * static_cast<float>(m_tileSize)
    };
}

sf::Vector2i GridSystem::worldToTile(sf::Vector2f pos) const
{
    return {
        static_cast<int>((pos.x - m_gridOrigin.x) / m_tileSize),
        static_cast<int>((pos.y - m_gridOrigin.y) / m_tileSize)
    };
}