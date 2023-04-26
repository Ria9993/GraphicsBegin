#include <PCH.h>
#include <Graphics/DX11Shader.h>

Shader::Shader()
{
}

Shader::~Shader()
{
}

VertexShader::VertexShader() : mVS(nullptr)
{
}

VertexShader::~VertexShader()
{
	SAFE_DELETE(mVS);
}

void VertexShader::Create(ID3DBlob* blob)
{
	HR(gDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &mVS));
}

void VertexShader::BindPipeline()
{
	if (mInputlayout) {
		gContext->IASetInputLayout(mInputlayout);
	}
	gContext->VSSetShader(mVS, 0, 0);
}

void CreateLayout(D3D11_INPUT_ELEMENT_DESC* elements, uint nElements,
	ID3DBlob* blob, ID3D11InputLayout** layout)
{
	gDevice->CreateInputLayout(
		elements,
		nElements,
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		layout);
}

void Compile(LPCSTR code, LPCSTR entry, LPCSTR version, ID3DBlob** blob)
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

std::vector<std::shared_ptr<Shader>> ShaderCache::mCache;

Shader* ShaderCache::CreateFromCode(const ShaderDesc& desc)
{
	ID3DBlob* blob = nullptr;
	std::shared_ptr<Shader> shader = nullptr;
	switch (desc.type)
	{
	case ShaderType::Vertex: {
		auto vs_shader = std::make_shared<VertexShader>();
		Compile(desc.code, "VS", "vs_5_0", &blob);
		CreateLayout(desc.elements, desc.nElements, blob, &vs_shader->mInputlayout);
		vs_shader->Create(blob);
		shader = vs_shader;
	}break;
	case ShaderType::Pixel: {
		shader = std::make_shared<PixelShader>();
		Compile(desc.code, "PS", "ps_5_0", &blob);
		shader->Create(blob);
	}break;
	default: {
	}break;
	}
	SAFE_RELEASE(blob);

	if (shader)
		return mCache.emplace_back(shader).get();
	return nullptr;
}

PixelShader::PixelShader() : mPS(nullptr)
{
}

PixelShader::~PixelShader()
{
	SAFE_RELEASE(mPS);
}

void PixelShader::Create(ID3DBlob* blob)
{
	gDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(),
		nullptr, &mPS);
}

void PixelShader::BindPipeline()
{
	gContext->PSSetShader(mPS, nullptr, 0);
}
