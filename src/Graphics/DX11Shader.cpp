#include <PCH.h>
#include <Graphics/DX11Shader.h>

Shader::Shader()
	: mVS(nullptr)
	, mPS(nullptr)
	, mVSBlob(nullptr)
{
}

Shader::~Shader()
{
	SAFE_RELEASE(mVS);
	SAFE_RELEASE(mPS);
	SAFE_RELEASE(mVSBlob);
}

void Shader::CreateFromCode(LPCSTR vs, LPCSTR ps)
{
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;

	Shader::Compile(vs, VS_ENTRY, VS_VERSION, &vsBlob);
	Shader::Compile(ps, PS_ENTRY, PS_VERSION, &psBlob);

	HR(gDevice->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		nullptr,
		&mVS));
	HR(gDevice->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		nullptr,
		&mPS));

	//SAFE_RELEASE(vsBlob);
	mVSBlob = vsBlob;
	SAFE_RELEASE(psBlob);
}

void Shader::Compile(LPCSTR code, LPCSTR entry, LPCSTR version, ID3DBlob** blob)
{
	ID3DBlob* errBlob{};
	HRESULT hr = D3DCompile(
		code, strlen(code),
		nullptr, nullptr, nullptr,
		entry, version,
		0, 0,
		blob, &errBlob
	);

	if (FAILED(hr)) {
		MessageBox(NULL, (LPCSTR)errBlob->GetBufferPointer(), "Shader Error", MB_OK);
	}
	SAFE_RELEASE(errBlob);
}

void Shader::Bind()
{
	gContext->VSSetShader(mVS, nullptr, 0);
	gContext->PSSetShader(mPS, nullptr, 0);
}
