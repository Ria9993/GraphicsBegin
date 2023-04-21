#pragma once

#include <Graphics/DX11Config.h>

#define gContext DX11Context1::g_Context->mContext
#define gDevice DX11Context1::g_Context->mDevice

class Context {
public:
	Context(int cx, int cy, HWND hwnd)
		: hwnd(hwnd), cx(cx), cy(cy) {}
	HWND hwnd;
	int cx;
	int cy;
};

class DX11Context1 : public Context
{
public:
	DX11Context1(int cx, int cy, HWND hwnd);
	~DX11Context1();

	void CreateDevice();
	void CreateSwapChain();
	void CreateRenderTargetView(int w, int h);
	void CreateState();

	void ClearBuffer(float r, float g, float b, float a);
	void SwapBuffer();

	//Device
	ID3D11Device1*			mDevice = nullptr;	
	ID3D11DeviceContext1*	mContext = nullptr;
	IDXGISwapChain1*		mSwapChain = nullptr;
	//View
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;
	ID3D11Texture2D* mDepthStencilBuffer = nullptr;


	//create state
	ID3D11RasterizerState* mRSState[2] = {};

	static DX11Context1* g_Context;

	bool mIsMsaaEnable = false;
	uint mMsaa = 0;
	bool mIsFlipModel = false;

};