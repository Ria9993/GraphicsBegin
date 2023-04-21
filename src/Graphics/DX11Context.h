#pragma once

#include <Graphics/DX11Config.h>

#define gContext DX11Context::g_Context->mContext.Get()
#define gDevice DX11Context::g_Context->mDevice.Get()

class Context {
public:
	Context(int cx, int cy, HWND hwnd) 
		: hwnd(hwnd), cx(cx), cy(cy) {}
	HWND hwnd;
	int cx;
	int cy;
};

class DX11Context : public Context
{
public:
	DX11Context(int cx, int cy, HWND hwnd);
	~DX11Context();

	void CreateDevice();
	void CreateSwapChain();
	void CreateState();

	void ClearBuffer(float r, float g, float b, float a);
	void SwapBuffer();

	//creator
	comptr<ID3D11Device1>			mDevice;
	//pipeline setting	
	comptr<ID3D11DeviceContext1>	mContext;
	//surface
	comptr<IDXGISwapChain1>			mSwapChain;
	//render target view
	ID3D11RenderTargetView*			mRTV = nullptr;


	//create state
	ID3D11RasterizerState*			mState;

	static DX11Context* g_Context;
};