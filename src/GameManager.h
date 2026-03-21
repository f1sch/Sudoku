#pragma once
#include "IUpdatable.h"

#include "GameLoop.h"

#include <memory>

class GameManager : public IUpdatable
{
public:
	GameManager();

	void Run();
private:
	void Update();
	void Render();

	std::unique_ptr<GameLoop> m_gameLoop;
};

