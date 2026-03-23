#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>

class Renderer
{
public:
	Renderer();

	const void Flush();
	std::vector<const sf::Drawable*>& GetQueue() { return m_queue; }
	sf::RenderWindow& GetWindow() { return m_window; }
private:
	sf::RenderWindow m_window;
	std::vector<const sf::Drawable*> m_queue;
};