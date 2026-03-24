#include "GridSystem.h"

#include <SFML/System/Vector2.hpp>

GridSystem::GridSystem(sf::Vector2f gridOrigin)
{
    m_gridOrigin = gridOrigin;
    m_cellSize = TEX_WIDTH;
    m_blockSpacing = 5.f;
}

sf::Vector2f GridSystem::GetCellPosition(int row, int col) const
{
    float originToRight = m_gridOrigin.x + col * m_cellSize + (col / 3) * m_blockSpacing;
    float offsetX = (m_cellSize*9 + m_blockSpacing) / 2;
    float x = originToRight - offsetX;
    
    float originToBottom = m_gridOrigin.y + row * m_cellSize + (row / 3) * m_blockSpacing;
    float offsetY = ((m_cellSize * 9) + m_blockSpacing) / 2;
    float y = originToBottom - offsetY;

    return sf::Vector2f(x,y);
}
