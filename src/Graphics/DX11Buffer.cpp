#include <PCH.h>
#include <Graphics/DX11Buffer.h>

Buffer::Buffer() 
	: mBuffer(nullptr)
{
}

Buffer::~Buffer()
{
	SAFE_RELEASE(mBuffer);
}

VertexBuffer::VertexBuffer(void* pData, uint bSize, uint stride)
	: mStride(stride)
{
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = bSize;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = pData;

	HR(gDevice->CreateBuffer(&desc, &sd, &mBuffer));
	//sd.
}

void VertexBuffer::Bind()
{
	uint offset = 0;
	gContext->IASetVertexBuffers(0, 1, &mBuffer, &mStride, &offset);
}
