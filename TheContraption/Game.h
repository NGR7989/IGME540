#pragma once

#define SHOW_GUI_ENTITIES 0
#define SHOW_GUI_LIGHTS 1
#define SHOW_GUI_CAMERA 2

#define SCENE_MAT_TEST 0
#define SCENE_DECONSTRUCTION 1

#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects

#include <memory>

#include <vector>
#include "Entity.h"
#include "Camera.h"

#include "SimpleShader.h"
#include "Material.h"
#include "packages/directxtk_desktop_win10.2023.9.6.1/include/WICTextureLoader.h"

#include "Lights.h"
#include "MatData.h"

#include "Sky.h"

#include "AnimCurves.h"
#include "Scenes.h"

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:
	void LoadLights();
	void LoadShaders(); 
	void CreateGeometry();
	void CreateCameras();

	// Gui - Used to tell the computer which gui to display 
	void UpdateImGui(float deltaTime);
	void UpdateEntityGUI();
	void UpdateLightGUI();
	void UpdateCameraGUI();

	// Following code is used to create specific GUI 
	void CreateEntityGui(std::shared_ptr<Entity> entity);
	/// <summary>
	/// Call this function to automatically create a light
	/// based on the lights type index
	/// </summary>
	/// <param name="light"></param>
	void CreateLightGui(Light* light);
	void CreateDirLightGui(Light *light);
	void CreatePointLightGui(Light *light);

	/// <summary>
	/// INteract with adjustable settings for given camera
	/// </summary>
	/// <param name="cam"></param>
	void CreateCamGui(Camera* cam);

	/// <summary>
	/// Creates a visual curve for ImGui
	/// </summary>
	void CreateCurveGui(int curveType, float plotSizeX = 100.0f, float plotSizeY = 80.0f);
	/// <summary>
	/// Create a curve gui that automatically creates the dropdown and will return the selected index 
	/// </summary>
	/// <returns>Selected index. See AnimCurves.h for defines</returns>
	int CreateCurveGuiWithDropDown(float plotSizeX = 100.0f, float plotSizeY = 80.0f);


	void SetupLitMaterial(
		std::shared_ptr<Material> mat, 
		const wchar_t albedoTextureAddress[],
		const wchar_t speculuarMapAddress[],
		const wchar_t normalMapAddress[],
		const char samplerType[] = "BasicSampler"
		);

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> rustyMetal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> rustyMetalSpec;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> blankNormal;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> rustyMetalSamplerState;


	// Shaders and shader-related constructs
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimplePixelShader> litShader;
	std::shared_ptr<SimplePixelShader> schlickShader;
	std::shared_ptr<SimplePixelShader> customPShader;
	std::shared_ptr<SimpleVertexShader> vertexShader;

	/// <summary>
	/// Holds all entities active in scene 
	/// </summary>
	//std::vector<std::shared_ptr<Entity>> entities;

	// Camera 
	//int currentCam;
	//std::vector<std::shared_ptr<Camera>> cameras; 

	// Materials 
	std::shared_ptr<Material> mat1;
	std::shared_ptr<Material> mat2;
	std::shared_ptr<Material> mat3;
	std::shared_ptr<Material> lit;
	std::shared_ptr<Material> schlickBricks;
	std::shared_ptr<Material> schlickCushions;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;

	std::unordered_map<std::shared_ptr<Material>, std::shared_ptr<MatData>> matToResources;

	// Light gizmo itmes 
	std::vector<std::shared_ptr<Entity>> lightGizmos;
	std::unordered_map<Light*, Entity*> lightToGizmos;
	
	// Changes between entities, lights, and camera 
	int currentGUI;

	// Lights 
	//std::vector<Light> directionalLights;
	//std::vector<Light> spotLights;

	//std::vector<Light> lights;
	Light directionalLight1;
	Light directionalLight2;
	Light directionalLight3;
	Light pointLight1;
	Light pointLight2;

	// Skybox 
	//std::shared_ptr<Sky> sky;

	std::shared_ptr<Scene> scene;
};

