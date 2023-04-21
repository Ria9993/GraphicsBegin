#include <PCH.h>
#include <Graphics/DX11InputLayout.h>

InputLayout::InputLayout()
	: mInputLayout(nullptr)
{

}

InputLayout::~InputLayout()
{
	SAFE_RELEASE(mInputLayout);
}

void InputLayout::Create(D3D11_INPUT_ELEMENT_DESC& desc)
{
	//desc.
}
