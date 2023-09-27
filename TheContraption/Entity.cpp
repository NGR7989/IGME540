#include "Entity.h"

Entity::Entity(std::shared_ptr<Mesh> model)
{
	transform = Transform();
	this->model = model;
}

std::shared_ptr<Mesh> Entity::GetModel()
{
	return model;
}

Transform* Entity::GetTransform() 
{ 
	return &transform; 
}

void Entity::Draw(
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, 
	Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer,
	std::shared_ptr<Camera> camera)
{
	VertexShaderExternalData vsData;
	vsData.colorTint = DirectX::XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
	vsData.world = transform.GetWorldMatrix();
	vsData.viewMatrix = *camera->GetViewMatrix().get();
	vsData.projMatrix = *camera->GetProjMatrix().get();

	D3D11_MAPPED_SUBRESOURCE mappedBuffer = {}; // Holds a memory position to the created resource 
	context->Map(vsConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer); // Lets is safely discard all data currently in buffer
	memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));
	context->Unmap(vsConstantBuffer.Get(), 0);

	context->VSSetConstantBuffers(
		0, // Which slot (register) to bind the buffer to?
		1, // How many are we activating? Can do multiple at once
		vsConstantBuffer.GetAddressOf()); // Array of buffers (or the address of one)

	model->Draw();
}
