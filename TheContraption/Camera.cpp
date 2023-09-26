#include "Camera.h"

Camera::Camera(
	float x, float y, float z, 
	float moveSpeed, 
	float mouseLookSpeed, 
	float fov, 
	float aspectRatio)
	:
	moveSpeed(moveSpeed),
	mouseLookSpeed(mouseLookSpeed)
{
	transform = new Transform();
	transform->SetPosition(x, y, z);
}

void Camera::Update(float dt)
{
}

void Camera::UpdateViewMatrix()
{
	// Setup
	DirectX::XMFLOAT3 pos = transform->GetPosition();
	DirectX::XMFLOAT3 fwd = transform->GetForward();

	// Build view and store 
	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(
		DirectX::XMLoadFloat3(&pos),
		DirectX::XMLoadFloat3(&fwd),
		DirectX::XMVectorSet(0, 1, 0, 0)
	);

	DirectX::XMStoreFloat4x4(&viewMatrix, view);
}

void Camera::UpdateProjMatrix(float fov, float aspectRatio)
{
	// Change clip planes to be paramters of constructor 
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.01f, 1000.0f);
	DirectX::XMStoreFloat4x4(&projMatrix, proj);
}

//DirectX::XMFLOAT4X4 Transform::GetViewMatrix()
//{
//	//return viewMatrix;
//}
//DirectX::XMFLOAT4X4 GetProjMatrix;

Transform* Camera::GetTransform()
{
	return nullptr;
}
