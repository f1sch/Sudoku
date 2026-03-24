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
	m_window.clear(sf::Color::White);
	for (const auto* obj : m_queue)
		m_window.draw(*obj);
	m_window.display();
	m_queue.clear();
}
