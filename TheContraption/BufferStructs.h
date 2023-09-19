#pragma once
#include <DirectXMath.h>


struct VertexShaderExternalData
{
	// Must match up with the vertex shader cbuffer
	DirectX::XMFLOAT4 colorTint;
	DirectX::XMFLOAT4X4 world;
};