#pragma once

#include <Graphics/DX11Context.h>

class Buffer
{
public:
	Buffer();
	virtual ~Buffer();

	ID3D11Buffer* mBuffer;
};

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(void* pData, uint bSize, uint stride);

	void Bind();

	uint mStride;
};