#pragma once

#include <Graphics/DX11Context1.h>

class InputLayout
{
public:
	InputLayout();
	virtual ~InputLayout();

	void Create(D3D11_INPUT_ELEMENT_DESC& desc);

	ID3D11InputLayout* mInputLayout;
};