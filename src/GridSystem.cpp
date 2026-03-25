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

    return { x, y };
}

//sf::Vector2f GridSystem::GetCellPosition(int row, int col) const
//{
//    float originToRight = m_gridOrigin.x + col * m_cellSize + (col / 3) * m_blockSpacing;
//    float offsetX = (m_cellSize*9 + m_blockSpacing) / 2;
//    float x = originToRight - offsetX;
//    
//    float originToBottom = m_gridOrigin.y + row * m_cellSize + (row / 3) * m_blockSpacing;
//    float offsetY = ((m_cellSize * 9) + m_blockSpacing) / 2;
//    float y = originToBottom - offsetY;
//
//    return sf::Vector2f(x,y);
//}
