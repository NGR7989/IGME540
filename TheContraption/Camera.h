#pragma once
#include "Transform.h";

class Camera
{
public:
	Camera(
		float x, float y, float z, 
		float moveSpeed, 
		float mouseLookSpeed, 
		float fov, 
		float aspectRatio);

	void Update(float dt);
	void UpdateViewMatrix();
	void UpdateProjMatrix(float fov, float aspectRatio);

	DirectX::XMFLOAT4X4 GetViewMatrix;
	DirectX::XMFLOAT4X4 GetProjMatrix;
	
private:
	// Change to shared_ptr 
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projMatrix;

	Transform* GetTransform();

	float moveSpeed;
	float mouseLookSpeed;
	Transform* transform;
};

