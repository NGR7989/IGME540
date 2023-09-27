#include "Camera.h"

Camera::Camera(
	float x, float y, float z, 
	float moveSpeed, 
	float mouseLookSpeed, 
	float fov, 
	float aspectRatio,
	float nearClip,
	float farClip)
	:
	moveSpeed(moveSpeed),
	mouseLookSpeed(mouseLookSpeed),
	nearClip(nearClip),
	farClip(farClip)
{
	transform = new Transform();
	transform->SetPosition(x, y, z);

	viewMatrix = std::make_shared<DirectX::XMFLOAT4X4>();
	projMatrix = std::make_shared<DirectX::XMFLOAT4X4>();

	UpdateProjMatrix(fov, aspectRatio);
}

Camera::~Camera()
{
	delete transform;
}

void Camera::Update(float dt)
{
	Input& input = Input::GetInstance();

	if (input.KeyDown('W')) 
	{
		transform->MoveRelative(0, 0, moveSpeed * dt);
	}
	if (input.KeyDown('S')) 
	{
		transform->MoveRelative(0, 0, -moveSpeed * dt);
	}


	if (input.MouseLeftDown())
	{
		float xDiff = mouseLookSpeed * input.GetMouseXDelta();
		float yDiff = mouseLookSpeed * input.GetMouseYDelta();
		// roate camera 

		transform->RotateEuler(yDiff * mouseLookSpeed, 0, 0);
		transform->RotateEuler(0, xDiff * mouseLookSpeed, 0);
	}

	if (input.KeyDown(VK_SHIFT)) { /* Shift is down */ }
	if (input.KeyDown(VK_CONTROL)) { /* Control is down */ }

	UpdateViewMatrix();
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

	DirectX::XMStoreFloat4x4(viewMatrix.get(), view);
}

void Camera::UpdateProjMatrix(float fov, float aspectRatio)
{
	// Change clip planes to be paramters of constructor 
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearClip, farClip);
	DirectX::XMStoreFloat4x4(projMatrix.get(), proj);
}

std::shared_ptr<DirectX::XMFLOAT4X4> Camera::GetViewMatrix()
{
	return viewMatrix;
}

std::shared_ptr<DirectX::XMFLOAT4X4> Camera::GetProjMatrix()
{
	return projMatrix;
}

Transform* Camera::GetTransform()
{
	return nullptr;
}
