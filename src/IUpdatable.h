#pragma once
class IUpdatable
{
public:
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual ~IUpdatable() = default;
};