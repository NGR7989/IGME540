#include "Material.h"

Material::Material(DirectX::XMFLOAT4 tint, std::shared_ptr<SimpleVertexShader> vertex, std::shared_ptr<SimplePixelShader> pixel) :
	tint(tint), vertex(vertex), pixel(pixel) {}

DirectX::XMFLOAT4 Material::GetTint()
{
	return tint;
}

std::shared_ptr<SimpleVertexShader> Material::GetVertexShader()
{
	return vertex;
}

std::shared_ptr<SimplePixelShader> Material::GetPixelShader()
{
	return pixel;
}



void Material::SetTint(DirectX::XMFLOAT4 nextTint)
{
	tint = nextTint;
}

void Material::SetVertexShader(std::shared_ptr<SimpleVertexShader> nextVertex)
{
	vertex = nextVertex;
}

void Material::SetPixelShader(std::shared_ptr<SimplePixelShader> nextPixel)
{
	pixel = nextPixel;
}