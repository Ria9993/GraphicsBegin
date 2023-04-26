#pragma once

#include <Graphics/DX11Context1.h>

class Buffer
{
public:
	Buffer();
	virtual ~Buffer();

	ID3D11Buffer* mBuffer;
	uint mId;
};

enum class BufferType : uint
{
	VERTEX, INDEX, CONSTANT
};

struct BufferDesc
{
	void* pData;
	uint cbSize;
	uint stride;
	BufferType type;
};

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(const BufferDesc& desc);

	void Bind();

	uint mStride;
};

class BufferCache
{
public:
	static VertexBuffer* CreateVertexBuffer(const BufferDesc& desc);
	static std::unordered_map<uint, std::unique_ptr<VertexBuffer>> mCache;
};