#pragma once

#include <Graphics/DX11Config.h>

struct VertexPC
{
	ver3f pos;
	ver3f col;

	static D3D11_INPUT_ELEMENT_DESC desc[];
	static uint nDesc;
};