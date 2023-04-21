#include <PCH.h>
#include <Graphics/DX11Context.h>

DX11Context* DX11Context::g_Context = nullptr;

DX11Context::DX11Context(int cx, int cy, HWND hwnd)
	: Context(cx, cy, hwnd)
{
	if (!g_Context) {
		g_Context = this;
	}
	CreateDevice();
	CreateSwapChain();
	CreateState();
}

DX11Context::~DX11Context()
{
}

void DX11Context::CreateDevice()
{
	HRESULT hr = NULL;
	comptr<ID3D11Device>		device;
	comptr<ID3D11DeviceContext>	context;

	uint createFlag = 0;
#ifdef _DEBUG
	createFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	D3D_FEATURE_LEVEL inFeatures[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};
	uint nInFeatures = ARRAYSIZE(inFeatures);

	D3D_FEATURE_LEVEL outFeature{};

	hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlag,
		inFeatures,
		nInFeatures,
		D3D11_SDK_VERSION,
		&device,
		&outFeature,
		&context
	);
	HR(hr);

	if (outFeature == D3D_FEATURE_LEVEL_11_1)
		printf("D3D Feature 11.1 supported \n");
	else if (outFeature == D3D_FEATURE_LEVEL_11_0)
		printf("D3D Feature 11.0 supported \n");

	device.As(&mDevice);
	context.As(&mContext);
}

void DX11Context::CreateSwapChain()
{
	//Device
	comptr<IDXGIDevice1> dxgiDevice;
	HR(mDevice.As(&dxgiDevice));
	//adapter
	comptr<IDXGIAdapter> adapter;
	HR(dxgiDevice->GetAdapter(adapter.GetAddressOf()));

	// why we use factory???????????
	comptr<IDXGIFactory4> factory{};
	HR(adapter->GetParent(__uuidof(IDXGIFactory1), (void**)factory.GetAddressOf()));

	DXGI_SWAP_CHAIN_DESC1 sd{};
	sd.Width = cx;
	sd.Height = cy;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	factory->CreateSwapChainForHwnd(
		mDevice.Get(), hwnd, &sd, nullptr, nullptr, mSwapChain.ReleaseAndGetAddressOf());

	//Back buffer
	ID3D11Texture2D* backBuffer;
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer));
	HR(mDevice->CreateRenderTargetView(backBuffer, nullptr, &mRTV));
	SAFE_RELEASE(backBuffer);

	//TODO: Depth stencil

	D3D11_VIEWPORT vp{};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = (float)cx;
	vp.Height = (float)cy;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	mContext->RSSetViewports(1, &vp);
	mContext->OMSetRenderTargets(1, &mRTV, nullptr);
	

}

void DX11Context::CreateState()
{
	D3D11_RASTERIZER_DESC rd{};
	rd.CullMode = D3D11_CULL_FRONT;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.FrontCounterClockwise = false;
	rd.DepthClipEnable = true;

	mDevice->CreateRasterizerState(&rd, &mState);
}

void DX11Context::ClearBuffer(float r, float g, float b, float a)
{
	const float buffer[4] = { r, g, b, a };
	mContext->ClearRenderTargetView(mRTV, buffer);
}

void DX11Context::SwapBuffer()
{
	mSwapChain->Present(0, 0);
}
