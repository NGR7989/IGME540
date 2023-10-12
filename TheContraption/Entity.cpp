#include "Entity.h"

Entity::Entity(std::shared_ptr<Mesh> model, std::shared_ptr<Material> mat) :
	model(model), mat(mat)
{
	transform = Transform();
}

std::shared_ptr<Mesh> Entity::GetModel()
{
	return model;
}

Transform* Entity::GetTransform() 
{ 
	return &transform; 
}

std::shared_ptr<Material> Entity::GetMat()
{
	return mat;
}

void Entity::SetMat(std::shared_ptr<Material> nextMat)
{
	mat = nextMat;
}

void Entity::Draw(
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, 
	std::shared_ptr<Camera> camera)
{
	mat->GetVertexShader()->SetShader();
	mat->GetPixelShader()->SetShader();


	//VertexShaderExternalData vsData;
	//vsData.colorTint = DirectX::XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
	//vsData.world = transform.GetWorldMatrix();
	//vsData.viewMatrix = *camera->GetViewMatrix().get();
	//vsData.projMatrix = *camera->GetProjMatrix().get();

	//D3D11_MAPPED_SUBRESOURCE mappedBuffer = {}; // Holds a memory position to the created resource 
	//context->Map(vsConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer); // Lets is safely discard all data currently in buffer
	//memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));
	//context->Unmap(vsConstantBuffer.Get(), 0);

	//context->VSSetConstantBuffers(
	//	0, // Which slot (register) to bind the buffer to?
	//	1, // How many are we activating? Can do multiple at once
	//	vsConstantBuffer.GetAddressOf()); // Array of buffers (or the address of one)



	std::shared_ptr<SimpleVertexShader> vs = mat->GetVertexShader();
	vs->SetFloat4("colorTint", mat->GetTint()); // Strings here MUST
	vs->SetMatrix4x4("world", transform.GetWorldMatrix()); // match variable
	vs->SetMatrix4x4("viewMatrix", *camera->GetViewMatrix().get()); // names in your
	vs->SetMatrix4x4("projMatrix", *camera->GetProjMatrix().get()); // shader’s cbuffer!

	vs->CopyAllBufferData();

	model->Draw();
}
