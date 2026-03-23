#pragma once
#include "IUpdatable.h"

class GameScene : public IUpdatable
{
public:
	GameScene();

	void Update() override;
	void Render() override;
};

