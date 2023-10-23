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
	moveSpeed(std::make_shared<float>(moveSpeed)),
	mouseLookSpeed(std::make_shared<float>(mouseLookSpeed)),
	nearClip(std::make_shared<float>(nearClip)),
	farClip(std::make_shared<float>(farClip))
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
		transform->MoveRelative(0, 0, *moveSpeed.get() * dt);
	}
	else if (input.KeyDown('S')) 
	{
		transform->MoveRelative(0, 0, -*moveSpeed.get() * dt);
	}

	if (input.KeyDown('E'))
	{
		transform->MoveRelative(0, *moveSpeed.get() * dt, 0);
	}
	else if (input.KeyDown('Q'))
	{
		transform->MoveRelative(0, -*moveSpeed.get() * dt, 0);
	}

	if (input.KeyDown('A'))
	{
		transform->MoveRelative(-*moveSpeed.get() * dt, 0, 0);
	}
	else if (input.KeyDown('D'))
	{
		transform->MoveRelative(*moveSpeed.get() * dt, 0, 0);
	}


	if (input.MouseLeftDown())
	{
		float xDiff = *mouseLookSpeed.get() * input.GetMouseXDelta();
		float yDiff = *mouseLookSpeed.get() * input.GetMouseYDelta();
		// roate camera 

		transform->RotateEuler( yDiff * *mouseLookSpeed.get(), 0, 0);
		transform->RotateEuler(0, xDiff * *mouseLookSpeed.get(), 0);
	}

	// Reset position 
	if (input.KeyDown(VK_SPACE))
	{
		transform->SetPosition(0, 0, -5);
	}

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	// Setup
	DirectX::XMFLOAT3 pos = *(transform->GetPosition().get());
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
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, *nearClip.get(), *farClip.get());
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
	return transform;
}
