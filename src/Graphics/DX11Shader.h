#pragma once

#include <Graphics/DX11Context1.h>

enum class ShaderType : uint
{
	Vertex,
	Pixel,
	Hull,
	Geometry
};

struct ShaderDesc
{
	ShaderType type;
	LPCSTR code;
	LPCSTR file;
	D3D11_INPUT_ELEMENT_DESC* elements;
	uint nElements;
};

class Shader {
public:
	Shader();
	~Shader();

	virtual void Create(ID3DBlob* blob) = 0;
	virtual void BindPipeline() = 0;
};

class VertexShader : public Shader 
{
public:
	VertexShader();
	virtual ~VertexShader();

	void Create(ID3DBlob* blob) override;
	void BindPipeline();

	ID3D11VertexShader* mVS;
	ID3D11InputLayout*	mInputlayout;
};

class PixelShader : public Shader
{
public:
	PixelShader();
	virtual ~PixelShader();

	void Create(ID3DBlob* blob) override;
	void BindPipeline();

	ID3D11PixelShader* mPS;
};

class ShaderCache {
public:
	static Shader* CreateFromCode(const ShaderDesc& desc);

	static std::vector<std::shared_ptr<Shader>> mCache;
};