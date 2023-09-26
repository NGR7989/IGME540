#pragma once
#include <memory>
#include <DirectXMath.h>

#include "Transform.h"
#include "Mesh.h"
#include "BufferStructs.h"
#include "Camera.h"


class Entity
{
private:
	Transform transform;
	std::shared_ptr<Mesh> model;
	
public:
	Entity(std::shared_ptr<Mesh> model);

	std::shared_ptr<Mesh> GetModel();
	Transform* GetTransform();

	// In the future this could be allocated to a rendering class that holds all drawing data intstead
	// of objects drawing themselves 
	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer, std::shared_ptr<Camera>);
};

