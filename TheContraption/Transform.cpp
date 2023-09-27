#include "Transform.h"

Transform::Transform() :
	position(0.0f, 0.0f, 0.0f),
	eulerRotation(0.0f, 0.0f, 0.0f),
	scale(1.0f, 1.0f, 1.0f)
{
	DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&worldTranspose, DirectX::XMMatrixIdentity());

	matIsDirty = true;
	dirIsDirty = true;
}

#pragma region HELPERS

void Transform::CleanMatrices()
{
	// Create a new world if transform has been mutated 
	if (matIsDirty)
	{
		// Get each of parts that represent the world matrix 
		DirectX::XMMATRIX pos =
			DirectX::XMMatrixTranslationFromVector(
				DirectX::XMLoadFloat3(&position));

		DirectX::XMMATRIX rot =
			DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&eulerRotation));

		DirectX::XMMATRIX sc =
			DirectX::XMMatrixScalingFromVector(
				XMLoadFloat3(&scale));

		// Update the matricies 
		DirectX::XMMATRIX wm = pos * rot * sc;
		DirectX::XMStoreFloat4x4(&world, wm);
		DirectX::XMStoreFloat4x4(&worldTranspose,
			DirectX::XMMatrixInverse(0, DirectX::XMMatrixTranspose(wm)));

		matIsDirty = false;
	}
}

void Transform::CleanVectors()
{
	if (!dirIsDirty)
		return;

	DirectX::XMVECTOR rotQuat = DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&eulerRotation));
	DirectX::XMStoreFloat3(&right, DirectX::XMVector3Rotate(DirectX::XMVectorSet(1, 0, 0, 0), rotQuat));
	DirectX::XMStoreFloat3(&up, DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 1, 0, 0), rotQuat));
	DirectX::XMStoreFloat3(&forward, DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), rotQuat));

	dirIsDirty = false;
}
#pragma endregion 

#pragma region SETTERS

void Transform::SetPosition(float x, float y, float z)
{
	// Was there a XMFloat function for this? 
	position.x = x;
	position.y = y;
	position.z = z;

	matIsDirty = true;
}

void Transform::SetPosition(DirectX::XMFLOAT3 position)
{
	this->position = position;

	matIsDirty = true;
}

void Transform::SetEulerRotation(float pitch, float yaw, float roll)
{
	eulerRotation.x = pitch;
	eulerRotation.y = yaw;
	eulerRotation.z = roll;

	matIsDirty = true;
	dirIsDirty = true;
}

void Transform::SetEulerRotation(DirectX::XMFLOAT3 rotation)
{
	this->eulerRotation = rotation;

	matIsDirty = true;
	dirIsDirty = true;
}

void Transform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;

	matIsDirty = true;
}

void Transform::SetScale(DirectX::XMFLOAT3 scale)
{
	this->scale = scale;

	matIsDirty = true;
}

void Transform::SetScale(float s)
{
	SetScale(s, s, s);

	matIsDirty = true;
}

#pragma endregion

#pragma region GETTERS
DirectX::XMFLOAT3 Transform::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Transform::GetEulerRotation()
{
	return eulerRotation;
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return scale;
}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	CleanMatrices();
	return world;
}

DirectX::XMFLOAT4X4 Transform::GetWorldInverseTransposeMatrix()
{
	CleanMatrices();
	return worldTranspose;
}

DirectX::XMFLOAT3 Transform::GetRight()
{
	if (dirIsDirty)
		CleanVectors();

	return right;
}

DirectX::XMFLOAT3 Transform::GetUp()
{
	if (dirIsDirty)
		CleanVectors();

	return up;
}

DirectX::XMFLOAT3 Transform::GetForward()
{
	if (dirIsDirty)
		CleanVectors();

	return forward;
}

#pragma endregion

#pragma region MUTATORS 
void Transform::MoveAbs(float x, float y, float z)
{
	this->position.x += x;
	this->position.y += y;
	this->position.z += z;
	matIsDirty = true;
}

void Transform::MoveAbs(DirectX::XMFLOAT3 offset)
{
	this->position.x += offset.x;
	this->position.y += offset.y;
	this->position.z += offset.z;
	matIsDirty = true;
}

void Transform::MoveRelative(float x, float y, float z)
{
	// Turn the euler angles into a quaternion 
	DirectX::XMVECTOR rotQuat = DirectX::XMQuaternionRotationRollPitchYawFromVector(
		DirectX::XMLoadFloat3(&eulerRotation)
	);
	
	// Creat the movement variable 
	DirectX::XMVECTOR toMove = DirectX::XMVectorSet(x, y, z, 0);

	// Convert to local space 
	toMove = DirectX::XMVector3Rotate(toMove, rotQuat);

	// Add in local space 
	toMove = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), toMove);

	// Store 
	DirectX::XMStoreFloat3(&position, toMove);
	matIsDirty = true;
}

void Transform::MoveRelative(DirectX::XMFLOAT3 vec)
{
	// Setup 
	DirectX::XMVECTOR rotEuler = DirectX::XMLoadFloat3(&eulerRotation);
	
	// Turn the euler angles into a quaternion 
	DirectX::XMVECTOR rotQuat = DirectX::XMQuaternionRotationRollPitchYaw(
		DirectX::XMVectorGetIntX(rotEuler),
		DirectX::XMVectorGetIntY(rotEuler),
		DirectX::XMVectorGetIntZ(rotEuler)
	);
	
	DirectX::XMVECTOR toMove = DirectX::XMLoadFloat3(&vec);

	// Convert to local space 
	toMove = DirectX::XMVector3Rotate(toMove, rotQuat);

	// Add in local space 
	toMove = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), toMove);

	// Store 
	DirectX::XMStoreFloat3(&position, toMove);

	matIsDirty = true;
}

void Transform::RotateEuler(float pitch, float yaw, float roll)
{
	eulerRotation.x += pitch;
	eulerRotation.y += yaw;
	eulerRotation.z += roll;

	matIsDirty = true;
	dirIsDirty = true;
}

void Transform::RotateEuler(DirectX::XMFLOAT3 rotation)
{
	eulerRotation.x += rotation.x;
	eulerRotation.y += rotation.y;
	eulerRotation.z += rotation.z;

	matIsDirty = true;
	dirIsDirty = true;
}

void Transform::Scale(float x, float y, float z)
{
	scale.x += x;
	scale.y += y;
	scale.z += z;

	matIsDirty = true;
}

void Transform::Scale(DirectX::XMFLOAT3 scale)
{
	this->scale.x += scale.x;
	this->scale.y += scale.y;
	this->scale.z += scale.z;

	matIsDirty = true;
}

void Transform::Scale(float scale)
{
	this->scale.x += scale;
	this->scale.y += scale;
	this->scale.z += scale;

	matIsDirty = true;
}

#pragma endregion
