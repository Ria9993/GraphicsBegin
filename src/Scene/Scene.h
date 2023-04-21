#pragma once

class Scene
{
public:
	Scene() = default;

	virtual bool Init() = 0;
	virtual void Render() = 0;
	virtual void Update(float dt) = 0;
};
