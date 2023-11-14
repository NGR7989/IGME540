#pragma once
#include "Entity.h"
#include "Lights.h"

/*
	The purpose of the script is to hold individual scene data and
	to draw the appropriate gui data 
*/

struct Scene
{
public:
	Scene(
		std::vector<std::shared_ptr<Camera>> cameras,
		std::vector<std::shared_ptr<Entity>> entities,
		std::vector<Light> lights
	);

private:
	// World entities 
	std::vector<std::shared_ptr<Entity>> entities;

	// Camera 
	int currentCam;
	std::vector<std::shared_ptr<Camera>> cameras;

	// Display light positions 
	std::vector<Light> directionalLights;
	std::vector<Light> spotLights;
	std::vector<std::shared_ptr<Entity>> lightGizmos;
	std::unordered_map<Light*, Entity*> lightToGizmos;
};