#pragma once
#include <SFML/System/Vector2.hpp>

class GridSystem
{
public:
	GridSystem(unsigned tileSize, sf::Vector2f gridOrigin);

	sf::Vector2f tileToWorld(int col, int row) const;
	sf::Vector2i worldToTile(sf::Vector2f pos) const;
private:
	unsigned m_tileSize;
	sf::Vector2f m_gridOrigin;
};