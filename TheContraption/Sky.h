#pragma once

#include <DirectXMath.h>
#include <wrl/client.h> 
#include <d3d11.h>
#include <memory>
#include <vector>

#include "Mesh.h"
#include "Material.h"
#include "SimpleShader.h"
#include "WICTextureLoader.h"
#include "PathHelpers.h"
#include "Camera.h"

class Sky
{
public:
	Sky(Microsoft::WRL::ComPtr<ID3D11Device> device, 
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler,
		std::shared_ptr<Mesh> skyMesh, 
		const wchar_t right[],
		const wchar_t left[],
		const wchar_t up[],
		const wchar_t down[],
		const wchar_t front[],
		const wchar_t back[],
		const wchar_t pixelShaderName[] = L"SkyPixelShader.cso",
		const wchar_t vertexShaderName[] = L"SkyVertexShader.cso"
	);
	//~Sky();

	void Draw(std::shared_ptr<Camera> camera);

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateCubemap(
		const wchar_t right[],
		const wchar_t left[],
		const wchar_t up[],
		const wchar_t down[],
		const wchar_t front[],
		const wchar_t back[]
	);
	
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubeMap;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> stencil;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer;

	std::shared_ptr<Mesh> skyMesh;
	std::shared_ptr<SimplePixelShader> skyps;
	std::shared_ptr<SimpleVertexShader> skyvs;
};