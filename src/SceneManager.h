#pragma once
#include "IInputListener.h"
#include "IUpdatable.h"

#include <memory>

class SceneManager : public IInputListener, public IUpdatable
{
public:
	SceneManager();

	void OnKeyPressed(sf::Keyboard::Key key) override;
	void Update() override;
	void Render() override;
private:
	std::unique_ptr<IUpdatable> m_scene;
};

