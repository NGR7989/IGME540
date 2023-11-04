#pragma once

struct MatData
{
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> albedo;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> spec;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normal;

	MatData(Microsoft::WRL::ComPtr<ID3D11Device> device, D3D11_SAMPLER_DESC sampDesc)
	{
		device.Get()->CreateSamplerState(&sampDesc, sampler.GetAddressOf());
	}
};