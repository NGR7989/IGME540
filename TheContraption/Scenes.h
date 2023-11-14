#pragma once
#include "Entity.h"


struct Scene
{
public:
	Scene();

private:
	std::vector<std::shared_ptr<Entity>> entities;

	// Camera 
	int currentCam;
	std::vector<std::shared_ptr<Camera>> cameras;
};