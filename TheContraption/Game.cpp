#include "Game.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"

#include <memory>
#include "Mesh.h"
#include "Transform.h"


// Assumes files are in "ImGui" subfolder!
// Adjust path as necessary
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// Direct3D itself, and our window, are not ready at this point!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,			// The application's handle
		L"DirectX Game",	// Text for the window's title bar (as a wide-character string)
		1280,				// Width of the window's client area
		720,				// Height of the window's client area
		false,				// Sync the framerate to the monitor refresh? (lock framerate)
		true)				// Show extra stats (fps) in title bar?
{
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");

	entities = std::vector<std::shared_ptr<Entity>>();
	cameras = std::vector<std::shared_ptr<Camera>>();
	currentCam = 0;
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Delete all objects manually created within this class
//  - Release() all Direct3D objects created within this class
// --------------------------------------------------------
Game::~Game()
{
	// Call delete or delete[] on any objects or arrays you've
	// created using new or new[] within this class
	// - Note: this is unnecessary if using smart pointers

	// Call Release() on any Direct3D objects made within this class
	// - Note: this is unnecessary for D3D objects stored in ComPtrs
	
	// ImGui clean up
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}

// --------------------------------------------------------
// Called once per program, after Direct3D and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	directionalLights = std::vector<Light>();

	directionalLight1 = {};
	directionalLight1.type = LIGHT_TYPE_DIRECTIONAL;
	directionalLight1.directiton = DirectX::XMFLOAT3(1, -1, 0);
	directionalLight1.color = DirectX::XMFLOAT3(1, 0, 0);
	directionalLight1.intensity = 1.0;
	directionalLights.push_back(directionalLight1);

	directionalLight2 = {};
	directionalLight2.type = LIGHT_TYPE_DIRECTIONAL;
	directionalLight2.directiton = DirectX::XMFLOAT3(0, -1, 0);
	directionalLight2.color = DirectX::XMFLOAT3(0, 0, 1);
	directionalLight2.intensity = 1.0;
	directionalLights.push_back(directionalLight2);

	directionalLight3 = {};
	directionalLight3.type = LIGHT_TYPE_DIRECTIONAL;
	directionalLight3.directiton = DirectX::XMFLOAT3(-0.2, 1, 0);
	directionalLight3.color = DirectX::XMFLOAT3(0, 1, 0);
	directionalLight3.intensity = 1.0;
	directionalLights.push_back(directionalLight3);
	

	spotLights = std::vector<Light>();

	spotLight1 = {};
	spotLight1.type = LIGHT_TYPE_SPOT;
	spotLight1.position = DirectX::XMFLOAT3(5, 5, 0);
	spotLight1.color = DirectX::XMFLOAT3(0, 1, 0);
	spotLight1.intensity = 1.0;
	spotLight1.range = 40.0;
	spotLights.push_back(spotLight1);

	spotLight2 = {};
	spotLight2.type = LIGHT_TYPE_SPOT;
	spotLight2.position = DirectX::XMFLOAT3(5, -5, 0);
	spotLight2.color = DirectX::XMFLOAT3(0, 0, 1);
	spotLight2.intensity = 1.0;
	spotLight2.range = 100.0;
	spotLights.push_back(spotLight2);

	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateGeometry();

	

	
	// Set initial graphics API state
	//  - These settings persist until we change them
	//  - Some of these, like the primitive topology & input layout, probably won't change
	//  - Others, like setting shaders, will need to be moved elsewhere later
	{
		// Tell the input assembler (IA) stage of the pipeline what kind of
		// geometric primitives (points, lines or triangles) we want to draw.  
		// Essentially: "What kind of shape should the GPU draw with our vertices?"
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Ensure the pipeline knows how to interpret all the numbers stored in
		// the vertex buffer. For this course, all of your vertices will probably
		// have the same layout, so we can just set this once at startup.
		//context->IASetInputLayout(inputLayout.Get());

	}


	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Pick a style (uncomment one of these 3)
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	//ImGui::StyleColorsClassic();
	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device.Get(), context.Get());


	// Create the cameras 
	cameras.push_back(std::make_shared<Camera>(
		0.0f, 0.0f, -5.0f,						// Pos
		1.0f,									// Move speed
		0.1f,									// Mouse look speed 
		XM_PIDIV4,								// FOV 
		this->windowWidth / this->windowHeight	// Aspect ratio 
		));

	cameras.push_back(std::make_shared<Camera>(
		0.0f, 0.0f, -20.0f,						// Pos
		3.0f,									// Move speed
		0.1f,									// Mouse look speed 
		XM_PIDIV4,								// FOV 
		this->windowWidth / this->windowHeight	// Aspect ratio 
		));

	cameras.push_back(std::make_shared<Camera>(
		2.0f, 1.5f, -15.0f,						// Pos
		1.0f,									// Move speed
		0.1f,									// Mouse look speed 
		XM_PI / 8,								// FOV 
		this->windowWidth / this->windowHeight	// Aspect ratio 
		));

	cameras.push_back(std::make_shared<Camera>(
		-1.0f, 1.0f, -5.0f,						// Pos
		1.0f,									// Move speed
		0.1f,									// Mouse look speed 
		XM_PIDIV2,								// FOV 
		this->windowWidth / this->windowHeight	// Aspect ratio 
		));
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = std::make_shared<SimpleVertexShader>(device, context,
		FixPath(L"VertexShader.cso").c_str());
	pixelShader = std::make_shared<SimplePixelShader>(device, context,
		FixPath(L"PixelShader.cso").c_str());
	customPShader = std::make_shared<SimplePixelShader>(device, context,
		FixPath(L"CustomPS.cso").c_str());
	litShader = std::make_shared<SimplePixelShader>(device, context,
		FixPath(L"litPS.cso").c_str());
}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in CPU memory
	//    over to a Direct3D-controlled data structure on the GPU (the vertex buffer)
	// - Note: Since we don't have a camera or really any concept of
	//    a "3d world" yet, we're simply describing positions within the
	//    bounds of how the rasterizer sees our screen: [-1 to +1] on X and Y
	// - This means (0,0) is at the very center of the screen.
	// - These are known as "Normalized Device Coordinates" or "Homogeneous 
	//    Screen Coords", which are ways to describe a position without
	//    knowing the exact size (in pixels) of the image/window/etc.  
	// - Long story short: Resizing the window also resizes the triangle,
	//    since we're describing the triangle in terms of the window itself
	Vertex vertices[] =
	{
		// Pos,	Normal, UV
		{ XMFLOAT3(+0.0f, +0.3f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
		{ XMFLOAT3(+0.3f, -0.3f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
		{ XMFLOAT3(-0.3f, -0.3f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
	};

	// Set up indices, which tell us which vertices to use and in which order
	// - This is redundant for just 3 vertices, but will be more useful later
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	unsigned int indices[] = { 0, 1, 2 };

	std::shared_ptr<Mesh> triangle = std::make_shared<Mesh>(device, context, vertices, indices, sizeof(vertices) / sizeof(Vertex), sizeof(indices) / sizeof(unsigned int));
	


	// Square 
	Vertex verticesB[] =
	{
		{ XMFLOAT3(-0.7f, +0.5f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
		{ XMFLOAT3(-0.4f, +0.5f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
		{ XMFLOAT3(-0.4f, -0.5f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
		{ XMFLOAT3(-0.7f, -0.5f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
	};
	unsigned int indicesB[] = { 0, 1, 2, 0, 2, 3 };

	std::shared_ptr<Mesh> square = std::make_shared<Mesh>(device, context, verticesB, indicesB, sizeof(verticesB) / sizeof(Vertex), sizeof(indicesB) / sizeof(unsigned int));


	Vertex verticesC[] =
	{
		{ XMFLOAT3(+0.8f, +0.8f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) }, // Center Top

		{ XMFLOAT3(+0.7f, +0.8f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
		{ XMFLOAT3(+0.65f, +0.9f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
		{ XMFLOAT3(+0.8f, +1.0f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
		{ XMFLOAT3(+0.95f, +0.9f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
		{ XMFLOAT3(+0.9f, +0.8f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
		
		{ XMFLOAT3(+0.6f, +0.1f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
		{ XMFLOAT3(+1.0f, +0.1f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
	};

	std::vector<unsigned int> hexVector = std::vector<unsigned int>();

	unsigned int indicesC[] = { 2, 0, 1,  2, 4, 0,  4, 5, 0,  1, 5, 6,  5, 7, 6};

	std::shared_ptr<Mesh> bow = std::make_shared<Mesh>(device, context, verticesC, indicesC, sizeof(verticesC) / sizeof(Vertex), sizeof(indicesC) / sizeof(unsigned int));


	mat1 = std::make_shared<Material>(DirectX::XMFLOAT4(1, 1, 1, 1), 1.0f, vertexShader, customPShader);
	mat2 = std::make_shared<Material>(DirectX::XMFLOAT4(1, 0, 1, 1), 1.0f, vertexShader, pixelShader);
	mat3 = std::make_shared<Material>(DirectX::XMFLOAT4(1, 1, 0, 1), 1.0f, vertexShader, pixelShader);
	lit = std::make_shared<Material>(DirectX::XMFLOAT4(1, 1, 0, 1), 0.5f, vertexShader, litShader);

	std::shared_ptr<Mesh> sphere = std::make_shared<Mesh>(device, context, FixPath(L"../../Assets/Models/sphere.obj").c_str());
	std::shared_ptr<Mesh> helix = std::make_shared<Mesh>(device, context, FixPath(L"../../Assets/Models/helix.obj").c_str());
	
	// Add all entites to the primary vector 
	entities.push_back(std::shared_ptr<Entity>(new Entity(helix, lit)));
	//entities.push_back(std::shared_ptr<Entity>(new Entity(triangle, mat2)));
	//entities.push_back(std::shared_ptr<Entity>(new Entity(square, mat3)));
	//entities.push_back(std::shared_ptr<Entity>(new Entity(square, mat1)));
	//entities.push_back(std::shared_ptr<Entity>(new Entity(bow, mat2)));
}


// --------------------------------------------------------
// Handle resizing to match the new window size.
//  - DXCore needs to resize the back buffer
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	cameras[currentCam].get()->UpdateProjMatrix(
		XM_PIDIV4,										// FOV 
		(float)this->windowWidth / this->windowHeight	// Aspect Ratio
	);
}


void Game::CreateEntityGui(std::shared_ptr<Entity> entity)
{
	Transform* trans = entity->GetTransform();
	XMFLOAT3 pos = *(trans->GetPosition().get());
	XMFLOAT3 rot = trans->GetEulerRotation();
	XMFLOAT3 sca = trans->GetScale();

	if (ImGui::DragFloat3("Position", &pos.x, 0.01f)) trans->SetPosition(pos);
	if (ImGui::DragFloat3("Rotation (Radians)", &rot.x, 0.01f)) trans->SetEulerRotation(rot);
	if (ImGui::DragFloat3("Scale", &sca.x, 0.01f)) trans->SetScale(sca);
}

void Game::CreateLightGui(Light *light)
{
	XMFLOAT3 color = light->color;
	XMFLOAT3 position = light->position;
	XMFLOAT3 direction = light->directiton;

	if (ImGui::DragFloat3("Color", &color.x, 0.01f)) light->color = color;
	if (ImGui::DragFloat3("Position", &position.x, 0.01f)) light->position = position;
	if (ImGui::DragFloat3("Direction", &direction.x, 0.01f)) light->directiton = direction;
}

void Game::UpdateImGui(float deltaTime)
{
	// Feed fresh input data to ImGui
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = deltaTime;
	io.DisplaySize.x = (float)this->windowWidth;
	io.DisplaySize.y = (float)this->windowHeight;


	// Reset the frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Determine new input capture
	Input& input = Input::GetInstance();
	input.SetKeyboardCapture(io.WantCaptureKeyboard);
	input.SetMouseCapture(io.WantCaptureMouse);

	float frameRate = ImGui::GetIO().Framerate;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
		1000.0 / frameRate, frameRate);
	ImGui::Text("Window Width: %i", windowWidth);
	ImGui::Text("Window Height: %i", windowHeight);

	// Display Entity data 
	if (ImGui::TreeNode("Entities"))
	{
		for (unsigned int i = 0; i < entities.size(); i++)
		{
			// Unique id
			ImGui::PushID(i);
			if (ImGui::TreeNode("Entity")) // How to make name based on id? 
			{
				CreateEntityGui(entities[i]);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Lights"))
	{
		for (unsigned int i = 0; i < directionalLights.size(); i++)
		{
			ImGui::PushID(i);
			if (ImGui::TreeNode("Directional"))
			{
				CreateLightGui(&directionalLights[i]);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}

		for (unsigned int i = 0; i < spotLights.size(); i++)
		{
			ImGui::PushID(i);
			if (ImGui::TreeNode("Point"))
			{
				CreateLightGui(&spotLights[i]);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}


	const char* items[] = { "Cam0", "Cam1", "Cam2", "Cam3"};
	static const char* current_item = items[0];

	if (ImGui::BeginCombo("Cameras", current_item)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item != items[n]); // New item 
			if (ImGui::Selectable(items[n], is_selected))
			{
				current_item = items[n];
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
					currentCam = n;
				}
			}
		}
		ImGui::EndCombo();
	}
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	UpdateImGui(deltaTime);
	float mouseLookSpeed = 2.0f;

	// Update the transform stuff for current assignment 
	//entities[0]->GetTransform()->SetPosition((float)cos(totalTime) / 2.0f, 0, 0);
	//entities[0]->GetTransform()->RotateEuler(0.0f, 0.0f, deltaTime * 2.0f);
	//entities[1]->GetTransform()->SetPosition(0.0f, 1.3f, 0.0f);
	//entities[1]->GetTransform()->SetScale((float)(cos(totalTime) + 1.1f) / 2.0f, (float)(sin(totalTime) + 1.5f) / 4.0f, 1.0f);
	//entities[2]->GetTransform()->MoveAbs(deltaTime * 0.2f, 0, 0);
	//entities[3]->GetTransform()->RotateEuler(0, 0, deltaTime * 0.5f);
	//entities[4]->GetTransform()->SetPosition((float)(sin(totalTime)), (float)(sin(totalTime)), 0);


	

	cameras[currentCam]->Update(deltaTime);

	// Example input checking: Quit if the escape key is pressed
	if (Input::GetInstance().KeyDown(VK_ESCAPE))
		Quit();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Frame START
	// - These things should happen ONCE PER FRAME
	// - At the beginning of Game::Draw() before drawing *anything*
	{
		// Clear the back buffer (erases what's on the screen)
		const float bgColor[4] = { 0.4f, 0.6f, 0.75f, 1.0f }; // Cornflower Blue
		context->ClearRenderTargetView(backBufferRTV.Get(), bgColor);

		// Clear the depth buffer (resets per-pixel occlusion information)
		context->ClearDepthStencilView(depthBufferDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		//if (i == 0) // JUST FOR THE NOISE SPHERE
		//{
		//	entities[i]->Draw(context, cameras[currentCam], totalTime);
		//	continue;
		//}

		DirectX::XMFLOAT3 ambient(0.1, 0.1, 0.25);
		entities[i]->GetMat()->GetPixelShader()->SetFloat3("ambient", ambient);

		for (int l = 0; l < directionalLights.size(); l++)
		{
			entities[i]->GetMat()->GetPixelShader()->SetData(
				"directionalLight" + std::to_string(l+1), // The name of the (eventual) variable in the shader
				&directionalLights[l], // The address of the data to set
				sizeof(Light)); // The size of the data (the whole struct!) to set
		}


		for (int l = 0; l < spotLights.size(); l++)
		{
			entities[i]->GetMat()->GetPixelShader()->SetData(
				"spotLight" + std::to_string(l + 1), // The name of the (eventual) variable in the shader
				&spotLights[l], // The address of the data to set
				sizeof(Light)); // The size of the data (the whole struct!) to set
		}



		entities[i]->Draw(context, cameras[currentCam]);
	}

	// Frame END
	// - These should happen exactly ONCE PER FRAME
	// - At the very end of the frame (after drawing *everything*)
	{
		// Present the back buffer to the user
		//  - Puts the results of what we've drawn onto the window
		//  - Without this, the user never sees anything
		bool vsyncNecessary = vsync || !deviceSupportsTearing || isFullscreen;


		// Draw ImGui
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


		swapChain->Present(
			vsyncNecessary ? 1 : 0,
			vsyncNecessary ? 0 : DXGI_PRESENT_ALLOW_TEARING);

		// Must re-bind buffers after presenting, as they become unbound
		context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthBufferDSV.Get());
	}
}