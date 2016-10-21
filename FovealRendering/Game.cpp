#include "Game.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	Meshes = 0;
	// Initialize fields

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	delete camera;
	delete renderEngine;
	// Release any (and all!) DirectX objects within Mesh
	// we've made in the Game class
	for (int i = 0; i < numMeshes; i++) {
		delete Meshes[i];
	}
	delete Meshes;

	delete[] Entity;

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete meshMaterial;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	camera = new Camera();
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();
	renderEngine = new Renderer(camera, context, backBufferRTV, depthStencilView);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	SimpleVertexShader* vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");		

	SimplePixelShader* pixelShader = new SimplePixelShader(device, context);
	if(!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))	
		pixelShader->LoadShaderFile(L"PixelShader.cso");

	// Adding the Texture stuff here....
	HRESULT result;
	ID3D11ShaderResourceView* tmpSRV;
	ID3D11ShaderResourceView* tmpNormSRV;
	ID3D11SamplerState* tmpSampler;

	result = CreateWICTextureFromFile(device, context, L"Debug/Textures/rock.jpg", 0, &tmpSRV);
	result = CreateWICTextureFromFile(device, context, L"Debug/Textures/rock_norm.jpg", 0, &tmpNormSRV);
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; // use the best
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &tmpSampler);

	// Create material with texture stuff
	meshMaterial = new Material(vertexShader, pixelShader, tmpSampler, tmpSRV, tmpNormSRV);
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Create the Projection matrix with appropriate aspect ratio
	camera->mkProjMat(width, height);
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	this->numMeshes = 8;
	// If we are worried about performance our mesh objects shouldn't follow pointers
	this->Meshes = new Mesh*[numMeshes];
	this->Meshes[0] = new Mesh("Debug/Assets/cone.obj", device);
	this->Meshes[1] = new Mesh("Debug/Assets/cube.obj", device);
	this->Meshes[2] = new Mesh("Debug/Assets/cylinder.obj", device);
	this->Meshes[3] = new Mesh("Debug/Assets/helix.obj", device);
	this->Meshes[4] = new Mesh("Debug/Assets/sphere.obj", device);
	this->Meshes[5] = new Mesh("Debug/Assets/torus.obj", device);
	this->Meshes[6] = new Mesh("Debug/Assets/low_cone.obj", device);
	this->Meshes[7] = new Mesh("Debug/Assets/high_cone.obj", device);

	// Let's try not to follow pointers
	this->numEntity = 6;
	// low, mid, high poly respectively
	Mesh** cones = new Mesh*[3];
	cones[0] = Meshes[3];
	cones[1] = Meshes[0];
	cones[2] = Meshes[4];

	this->Entity = new GameEntity[numEntity]{
		//GameEntity(Meshes[0], meshMaterial),
		GameEntity(cones, meshMaterial),
		GameEntity(Meshes[1], meshMaterial),
		GameEntity(Meshes[2], meshMaterial),
		GameEntity(Meshes[3], meshMaterial),
		GameEntity(Meshes[4], meshMaterial),
		GameEntity(Meshes[5], meshMaterial)
	};
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	camera->mkProjMat(width, height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	const float amount = deltaTime*5;

	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	// Check for poly level
	Entity[modelChoice].WhichPoly(camera);

	if (GetAsyncKeyState('W') & 0x8000)
		camera->Forward(amount);
	if (GetAsyncKeyState('S') & 0x8000)
		camera->Backward(amount);
	if (GetAsyncKeyState('A') & 0x8000)
		camera->StrafeLeft(amount);
	if (GetAsyncKeyState('D') & 0x8000)
		camera->StrafeRight(amount);

	if (GetAsyncKeyState(' ') & 0x8000 || GetAsyncKeyState('Q') & 0x8000)
		camera->MoveUp(amount);
	if (GetAsyncKeyState('X') & 0x8000 || GetAsyncKeyState('E') & 0x8000)
		camera->MoveDown(amount);

	// Check for entity swap
	bool currTab = (GetAsyncKeyState('	') & 0x8000) != 0;
	if (currTab && !prevTab)
		modelChoice = (modelChoice + 1) % numEntity;
		// check and update poly level mesh depending on camera location
		//Entity[modelChoice].WhichPoly();
	prevTab = currTab;

	// RESET THE CAMERA
	if (GetAsyncKeyState('R') & 0x8000)
		camera->ResetCamera();

}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	renderEngine->DrawOneMaterial(&Entity[modelChoice], 1, deltaTime, totalTime);

	// Swap back and front
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	if (buttonState & 0x0001 || buttonState & 0x0002) {
		// THESE ARE CORRECT
		// movement of the mouse on the x axis shouls rotate around Y
		// And Vice Versa
		camera->AddYRot((x-prevMousePos.x)*0.005);
		camera->AddXRot((y-prevMousePos.y)*0.005);
	}

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion