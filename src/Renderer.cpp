#include "Renderer.h"

#include <shared/Data.h>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/VideoMode.hpp>

Renderer::Renderer()
	: m_queue{}
{
	m_window = sf::RenderWindow(sf::VideoMode({ 1280u, 720u }), "Sudoku");
	m_window.setFramerateLimit(60);

	m_view.setSize({ VIRTUAL_WIDTH, VIRTUAL_HEIGHT });
	m_view.setCenter({ VIRTUAL_WIDTH / 2.f, VIRTUAL_HEIGHT / 2.f });

	m_window.setView(m_view);

	handleResize(m_window.getSize().x, m_window.getSize().y);
}

const void Renderer::flush()
{
	m_window.clear(sf::Color::White);
	for (const auto* obj : m_queue)
		m_window.draw(*obj);
	
	m_window.display();
	
	m_queue.clear();
}

void Renderer::handleResize(unsigned windowWidth, unsigned windowHeight)
{
	float windowRatio = static_cast<float>(windowWidth) / windowHeight;
	float viewRatio = static_cast<float>(VIRTUAL_WIDTH) / VIRTUAL_HEIGHT;

	float sizeX = 1.0f, sizeY = 1.0f;
	float posX = 0.f, posY = 0.f;

	if (windowRatio > viewRatio)
	{
		sizeX = viewRatio / windowRatio;
		posX = (1.f - sizeX) / 2.f;
	}
	else
	{
		sizeY = windowRatio / viewRatio;
		posY = (1.f - sizeY) / 2.f;
	}

	m_view.setViewport({ { posX, posY }, { sizeX, sizeY } });
	m_window.setView(m_view);
}