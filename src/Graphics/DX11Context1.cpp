#include <PCH.h>
#include <Graphics/Dx11Context1.h>

DX11Context1* DX11Context1::g_Context = nullptr;

DX11Context1::DX11Context1(int cx, int cy, HWND hwnd)
	: Context(cx, cy, hwnd)
{
	if (!g_Context) g_Context = this;
	CreateDevice();
	CreateSwapChain();
	CreateRenderTargetView(cx, cy);
	CreateState();
}

DX11Context1::~DX11Context1()
{
	mContext->ClearState();
	for (int i = 0; i < ARRAYSIZE(mRSState); i++)
		SAFE_RELEASE(mRSState[i]);
	SAFE_RELEASE(mDepthStencilBuffer);
	SAFE_RELEASE(mDepthStencilView);
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mSwapChain);
	SAFE_RELEASE(mContext);
	SAFE_RELEASE(mDevice);
}

void DX11Context1::CreateDevice()
{
	uint flag = 0;
#ifdef _DEBUG
	flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif //_DEBUG
	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
	uint nDriverType = 1;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};
	uint nFeatureLevels = ARRAYSIZE(featureLevels);
	HRESULT hr = S_OK;
	D3D_FEATURE_LEVEL outputFeatureLevel{};

	ID3D11Device*			localDevice = nullptr;
	ID3D11DeviceContext*	localContext = nullptr;

	hr = D3D11CreateDevice(
		nullptr,
		driverType,
		nullptr,
		flag,
		featureLevels,
		nFeatureLevels,
		D3D11_SDK_VERSION,
		&localDevice,
		&outputFeatureLevel,
		&localContext);

	//11.0 to 11.1
	HR(localDevice->QueryInterface(IID_PPV_ARGS(&mDevice)));
	HR(localContext->QueryInterface(IID_PPV_ARGS(&mContext)));

	SAFE_RELEASE(localDevice);
	SAFE_RELEASE(localContext);

	mDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &mMsaa);
	
	if (mIsMsaaEnable) {
		LOG << "MSAA Sampling : " << mMsaa << ENDL;
	}
}

void DX11Context1::CreateSwapChain()
{
	IDXGIDevice* dxgiDevice = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;
	IDXGIFactory1* dxgiFactory1 = nullptr;
	IDXGIFactory2* dxgiFactory2 = nullptr;

	//Device
	HR(mDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

	//Apater
	HR(dxgiDevice->GetAdapter(&dxgiAdapter));

	//Factory
	HR(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory1)));

	HRESULT hr = dxgiFactory1->QueryInterface(IID_PPV_ARGS(&dxgiFactory2));
	if (FAILED(hr))
		LOG_ERROR("DirectX doesnt support 11.1 %d\n");

	// 11.1
	DXGI_SWAP_CHAIN_DESC1 sd{};
	sd.Width              = cx;
	sd.Height             = cy;
	sd.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count   = mIsMsaaEnable ? 4 : 1;
	sd.SampleDesc.Quality = mIsMsaaEnable ? mMsaa - 1 : 0;
	sd.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount        = mIsFlipModel ? 2 : 1;
	sd.SwapEffect         = mIsFlipModel ? DXGI_SWAP_EFFECT_FLIP_DISCARD : DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags              = 0;
	sd.Scaling            = DXGI_SCALING_STRETCH;
	sd.AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fd{};
	fd.RefreshRate.Numerator   = 60;
	fd.RefreshRate.Denominator = 1;
	fd.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
	fd.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	fd.Windowed                = TRUE;

	HR(dxgiFactory2->CreateSwapChainForHwnd(mDevice, hwnd, &sd, &fd, nullptr, &mSwapChain));

	SAFE_RELEASE(dxgiFactory2);
	SAFE_RELEASE(dxgiFactory1);
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiDevice);
}

void DX11Context1::CreateRenderTargetView(int w, int h)
{
	if (!mDevice) LOG_ERROR("failed to find D3D Device");
	cx = w;
	cy = h;
	SAFE_RELEASE(mDepthStencilBuffer);
	SAFE_RELEASE(mDepthStencilView);
	SAFE_RELEASE(mRenderTargetView);
	
	ID3D11Texture2D* backBuffer = nullptr;
	HR(mSwapChain->ResizeBuffers(1, cx, cy, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	HR(mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	HR(mDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView));
	SAFE_RELEASE(backBuffer);

	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	depthStencilDesc.Width              = cx;
	depthStencilDesc.Height             = cy;
	depthStencilDesc.MipLevels          = 1;
	depthStencilDesc.ArraySize          = 1;
	depthStencilDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = mIsMsaaEnable ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = mIsMsaaEnable ? mMsaa - 1 : 0;
	depthStencilDesc.Usage              = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
	HR(mDevice->CreateTexture2D(&depthStencilDesc, nullptr, &mDepthStencilBuffer));
	HR(mDevice->CreateDepthStencilView(mDepthStencilBuffer, nullptr, &mDepthStencilView));

	mContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	D3D11_VIEWPORT vp{};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width    = (float)cx;
	vp.Height   = (float)cy;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	mContext->RSSetViewports(1, &vp);
}

void DX11Context1::CreateState()
{
	D3D11_RASTERIZER_DESC rd{};
	rd.CullMode = D3D11_CULL_FRONT;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.FrontCounterClockwise = true;
	rd.DepthClipEnable = true;

	HR(mDevice->CreateRasterizerState(&rd, &mRSState[0]));

	rd.FillMode = D3D11_FILL_WIREFRAME;
	HR(mDevice->CreateRasterizerState(&rd, &mRSState[1]));

	mContext->RSSetState(mRSState[1]);
}

void DX11Context1::ClearBuffer(float r, float g, float b, float a)
{
	const float rgba[] = { r, g, b, a };
	mContext->ClearRenderTargetView(mRenderTargetView, rgba);
	mContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DX11Context1::SwapBuffer()
{
	mSwapChain->Present(0, 0);
}
