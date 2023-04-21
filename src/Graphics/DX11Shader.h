#pragma once

#include <Graphics/DX11Context.h>

#define VS_ENTRY "VS"
#define PS_ENTRY "PS"
#define VS_VERSION "vs_5_0"
#define PS_VERSION "ps_5_0"

class Shader {
public:
	Shader();
	virtual ~Shader();

	void CreateFromCode(LPCSTR vs, LPCSTR ps);

	static void Compile(LPCSTR code, LPCSTR entry, LPCSTR version, ID3DBlob** blob);

	void Bind();

	ID3D11VertexShader* mVS;
	ID3D11PixelShader*	mPS;
	ID3DBlob*			mVSBlob;
};