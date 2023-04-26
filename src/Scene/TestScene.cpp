#include <PCH.h>
#include <Scene/TestScene.h>
#include <Graphics/DX11Buffer.h>

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
	, mPS(nullptr)
	, mVS(nullptr)
{
}

TestScene::~TestScene()
{
	//SAFE_DELETE(mIL);
	//SAFE_DELETE(mPS);
	//SAFE_DELETE(mVS);
	SAFE_DELETE(mVertexBuffer);
}

bool TestScene::Init()
{
	//vertex
	VertexPC vertices[] = {
		VertexPC{{0.0f, 0.5f, 0.0f}, {1, 0, 0}},
		VertexPC{{0.5f,-0.5f, 0.0f}, {0, 1, 0}},
		VertexPC{{-0.5f, -0.5f, 0.0f}, {0, 0, 1}}
	};

	BufferDesc desc{};
	desc.pData = vertices;
	desc.cbSize = ARRAYSIZE(vertices) * sizeof(Vertex);
	desc.stride = sizeof(Vertex);
	desc.type = BufferType::VERTEX;
	mVertexBuffer = BufferCache::CreateVertexBuffer(desc);

	//shader

	//input layout

	ShaderDesc sd{};
	sd.code = vsCode;
	sd.type = ShaderType::Vertex;
	sd.elements = VertexPC::desc;
	sd.nElements = VertexPC::nDesc;

	mVS = ShaderCache::CreateFromCode(sd);

	sd.code = psCode;
	sd.type = ShaderType::Pixel;
	mPS = ShaderCache::CreateFromCode(sd);

	mVS->BindPipeline();
	mPS->BindPipeline();

	gContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mVertexBuffer->Bind();

	return true;
}

void TestScene::Render()
{
	gContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mVertexBuffer->Bind();

	gContext->Draw(3, 0);
}

void TestScene::Update(float dt)
{
}
