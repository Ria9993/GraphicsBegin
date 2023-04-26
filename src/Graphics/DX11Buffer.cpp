#include <PCH.h>
#include <Graphics/DX11Buffer.h>

std::unordered_map<uint, std::unique_ptr<VertexBuffer>> BufferCache::mCache;

Buffer::Buffer() 
	: mBuffer(nullptr)
{
	static uint counter = 0;
	mId = counter++;
}

Buffer::~Buffer()
{
	SAFE_RELEASE(mBuffer);
}

VertexBuffer::VertexBuffer(const BufferDesc& desc)
	: mStride(desc.stride)
{
	D3D11_BUFFER_DESC bd{};
	bd.ByteWidth = desc.cbSize;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = desc.pData;

	HRESULT hr = gDevice->CreateBuffer(&bd, &sd, &mBuffer);
	
	if (SUCCEEDED(hr)) {
		printf("create id : %d buffer successed\n", mId);
	}
}

void VertexBuffer::Bind()
{
	uint offset = 0;
	gContext->IASetVertexBuffers(0, 1, &mBuffer, &mStride, &offset);
}

VertexBuffer* BufferCache::CreateVertexBuffer(const BufferDesc& desc)
{
	std::unique_ptr<VertexBuffer> bufferPtr = std::make_unique<VertexBuffer>(desc);
	VertexBuffer* buffer = nullptr;
	const uint id = bufferPtr->mId;
	if (mCache.find(bufferPtr->mId) == mCache.end())
	{
		mCache[id] = std::move(bufferPtr);
	}

	return mCache[id].get();;
}
