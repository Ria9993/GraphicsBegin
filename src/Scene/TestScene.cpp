#include <PCH.h>
#include <Scene/TestScene.h>
#include <Graphics/DX11Buffer.h>
#include <Graphics/DX11InputLayout.h>

LPCSTR vsCode = R"(
struct VS_In
{
    float3 pos : POSITION;
	float3 col : COLOR;
};

struct PS_In
{
    float4 pos : SV_POSITION;
	float4 col : COLOR;
};

PS_In VS(VS_In vs)
{
    PS_In ps;
    ps.pos = float4(vs.pos, 1);
	ps.col = float4(vs.col, 1);
    return ps;
};
)";

LPCSTR psCode = R"(
struct PS_In
{
    float4 pos : SV_POSITION;
	float4 col : COLOR;
};

float4 PS(PS_In ps) : SV_TARGET
{
	return ps.col;
};
)";

TestScene::TestScene()
	: mVertexBuffer(nullptr)
	, mShader(nullptr)
	, mIL(nullptr)
{
}

TestScene::~TestScene()
{
	SAFE_DELETE(mIL);
	SAFE_DELETE(mShader);
	SAFE_DELETE(mVertexBuffer);
}

bool TestScene::Init()
{
	//vertex
	Vertex vertices[] = {
		Vertex{{0.0f, 0.5f, 0.0f}, {1, 0, 0}},
		Vertex{{0.5f,-0.5f, 0.0f}, {0, 1, 0}},
		Vertex{{-0.5f, -0.5f, 0.0f}, {0, 0, 1}}
	};

	uint stride = sizeof(Vertex);
	mVertexBuffer = new VertexBuffer(vertices, ARRAYSIZE(vertices) * sizeof(Vertex), stride);

	//shader
	mShader = new Shader();
	mShader->CreateFromCode(vsCode, psCode);

	//input layout
	D3D11_INPUT_ELEMENT_DESC ed[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	mIL = new InputLayout();
	HRESULT hr = gDevice->CreateInputLayout(
		ed,
		ARRAYSIZE(ed),
		mShader->mVSBlob->GetBufferPointer(),
		mShader->mVSBlob->GetBufferSize(),
		&mIL->mInputLayout);
	if (FAILED(hr))
		return false;


	return true;
}

void TestScene::Render()
{
	gContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mVertexBuffer->Bind();
	mShader->Bind();
	gContext->IASetInputLayout(mIL->mInputLayout);

	gContext->Draw(3, 0);
}

void TestScene::Update(float dt)
{
}
