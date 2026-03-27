#include "Renderer.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/VideoMode.hpp>

Renderer::Renderer()
	: m_queue{}
{
	m_window = sf::RenderWindow(sf::VideoMode({ 1280, 720 }), "Sudoku");
}

const void Renderer::Flush()
{
	// TODO: of all the sf::Drawables, only sf::Sprites can render a sub-rectangle from a texture
	// check obj for type and render sub-rectangle if sf::Sprite? when to render sub-rectangle and
	// when to render whole texture? 
	// std::vector<int> subIndices; if subIndices.size == 0 then render whole texture
	m_window.clear(sf::Color::White);
	for (const auto* obj : m_queue)
		m_window.draw(*obj);
	m_window.display();
	m_queue.clear();
}
