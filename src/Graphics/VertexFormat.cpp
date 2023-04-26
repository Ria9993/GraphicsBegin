#include <PCH.h>
#include <Graphics/VertexFormat.h>

D3D11_INPUT_ELEMENT_DESC VertexPC::desc[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPC, VertexPC::col), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

uint VertexPC::nDesc = 2;