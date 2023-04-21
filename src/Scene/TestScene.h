#pragma once

#include <Scene/Scene.h>
#include <Graphics/DX11Shader.h>

struct Vertex
{
	ver3f pos;
	ver3f col;
};

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	virtual bool Init();
	virtual void Render();
	virtual void Update(float dt);

	VertexBuffer*	mVertexBuffer;
	Shader*			mShader;
	InputLayout*	mIL;
};