#pragma once


#include <d3d11.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include "Vertex.h"

class Mesh
{
private:
	// Buffers that connect data to the GPU 
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

public:
	Mesh(Microsoft::WRL::ComPtr<ID3D11Device> device, Vertex vertices[], unsigned int indices[]);
	~Mesh();

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();

	void Draw(float deltaTime, float totalTime);
};

