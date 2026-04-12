#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <vector>

class Renderer
{
public:
	Renderer();

	const void flush();
	void handleResize(unsigned windowWidth, unsigned windowHeight);

	sf::RenderWindow m_window;
	std::vector<const sf::Drawable*> m_queue;

private:
	sf::View m_view;
};