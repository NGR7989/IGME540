#pragma once
#include <DirectXMath.h>
#include <memory>

#include "SimpleShader.h";

class Material
{
public:
	Material(DirectX::XMFLOAT4 tint, std::shared_ptr<SimpleVertexShader> vertex, std::shared_ptr<SimplePixelShader> pixel);

	/// <summary>
	/// Get this materials current color tint 
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT4 GetTint();
	/// <summary>
	/// Get this material's current vertex shader shared pointer 
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<SimpleVertexShader> GetVertexShader();
	/// <summary>
	/// Get this material's current pixel shader shared pointer 
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<SimplePixelShader> GetPixelShader();

	/// <summary>
	/// Set this material's current color tint 
	/// </summary>
	/// <param name="nextTint"></param>
	void SetTint(DirectX::XMFLOAT4 nextTint);

	/// <summary>
	/// Set this material's current vertex shader 
	/// </summary>
	/// <param name="nextVertex"></param>
	void SetVertexShader(std::shared_ptr<SimpleVertexShader> nextVertex);

	/// <summary>
	/// Set this materials current pixel shader 
	/// </summary>
	/// <param name="nextPixel"></param>
	void SetPixelShader(std::shared_ptr<SimplePixelShader> nextPixel);

private:
	DirectX::XMFLOAT4 tint;

	std::shared_ptr<SimpleVertexShader> vertex;
	std::shared_ptr<SimplePixelShader> pixel;
};

