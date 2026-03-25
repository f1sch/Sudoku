#pragma once
#include <SFML/System/Vector2.hpp>

auto constexpr TEX_WIDTH = 32;
auto constexpr TEX_HEIGHT = 32;

class GridSystem
{
public:
	GridSystem(int tileSize, sf::Vector2f gridOrigin);

	sf::Vector2f tileToWorld(int tileX, int tileY) const;
	sf::Vector2f worldToTile(sf::Vector2f pos) const;

	//sf::Vector2f GetCellPosition(int row, int col) const;
private:
	int m_tileSize;
	sf::Vector2f m_gridOrigin;
	//float m_cellSize;
	//float m_blockSpacing;
};