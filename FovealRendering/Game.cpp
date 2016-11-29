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
	delete eyeTracker;
	delete camera;
	delete renderEngine;
	// Release any (and all!) DirectX objects within Mesh
	// we've made in the Game class

	for (int i = 0; i < 3; i++) {
		delete multiPoly[i];
	}
	delete multiPoly;

	delete[] Entity;

	delete[] PointLights;
	delete[] DirLights;

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	tmpSampler->Release();
	delete meshMaterial;
	delete noMaterial;
	delete floorMaterial;
	delete wallMaterial;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	eyeTracker = new EyeTracker(width, height);
	camera = new Camera();
	camera->SetPos(XMFLOAT3(0, 3, -10));
	renderEngine = new FovealRenderer(camera, device, context, backBufferRTV, depthStencilView, width, height);
	//renderEngine = new Renderer(camera, device, context, backBufferRTV, depthStencilView);

	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();
	
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
	renderEngine->AddVertexShader("default", L"VertexShader.cso");
	renderEngine->AddPixelShader("default", L"PixelShader.cso");
	renderEngine->AddPixelShader("none", L"NoMaterialShader.cso");

	// Create The Shaders for deffered
	renderEngine->AddVertexShader("gBuffer", L"gBufferVertexShader.cso");
	renderEngine->AddPixelShader("gBuffer", L"gBufferPixelShader.cso");

	renderEngine->AddVertexShader("fovealMask", L"fovealMask.cso");

	renderEngine->AddVertexShader("quad", L"quadVertexShader.cso");
	renderEngine->AddPixelShader("quad", L"quadPixelShader.cso");
	renderEngine->AddPixelShader("sphereLight", L"sphereLightPixelShader.cso");

	// Adding the Texture stuff here....
	HRESULT result;
	ID3D11ShaderResourceView* tmpSRV;

	result = CreateWICTextureFromFile(device, context, L"Debug/Textures/marble.jpg", 0, &tmpSRV);
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; // use the best
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &tmpSampler);

	// Create material with texture stuff
	meshMaterial = new Material(tmpSampler, tmpSRV);
	noMaterial = new Material("none");

	result = CreateWICTextureFromFile(device, context, L"Debug/Textures/floorTile.jpg", 0, &tmpSRV);
	floorMaterial = new Material(tmpSampler, tmpSRV);

	result = CreateWICTextureFromFile(device, context, L"Debug/Textures/wall.jpg", 0, &tmpSRV);
	wallMaterial = new Material(tmpSampler, tmpSRV);
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
	renderEngine->AddMesh("sphere", new Mesh("Debug/Assets/sphere.obj", device));
	renderEngine->AddMesh("quad",  new Mesh(device));

	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);


	renderEngine->AddMesh("cube", new Mesh("Debug/Assets/cube.obj", device));
	//this->Meshes[6] = new Mesh("Debug/Assets/bunny.obj", device);
	//this->Meshes[7] = new Mesh("Debug/Assets/armadillo.obj", device);

	// low, mid, high poly respectively
	this->multiPoly = new Mesh*[3];
	multiPoly[0] = new Mesh("Debug/Assets/low_bunny.obj", device);
	multiPoly[1] = new Mesh("Debug/Assets/mid_bunny.obj", device);
	multiPoly[2] = new Mesh("Debug/Assets/bunny.obj", device);
	
	/*
	multiPoly[0] = Meshes[0];
	multiPoly[1] = Meshes[3];
	multiPoly[2] = Meshes[4];
	*/
	// Let's try not to follow pointers
	this->numEntity = 7;
	this->Entity = new GameEntity[numEntity];

	// floor
	this->Entity[0] = GameEntity(renderEngine->GetMesh("cube"), floorMaterial);
	this->Entity[0].ScaleTo(XMFLOAT3(50, 1, 50));
	this->Entity[0].TranslateTo(XMFLOAT3(0, -1, 0));

	// wall 1
	this->Entity[1] = GameEntity(renderEngine->GetMesh("cube"), wallMaterial);
	this->Entity[1].ScaleTo(XMFLOAT3(50, 25, 1));
	this->Entity[1].TranslateTo(XMFLOAT3(0, 10, 25));
	// wall 2
	this->Entity[2] = GameEntity(renderEngine->GetMesh("cube"), wallMaterial);
	this->Entity[2].ScaleTo(XMFLOAT3(50, 25, 1));
	this->Entity[2].TranslateTo(XMFLOAT3(0, 10, -25));
	
	// wall 3
	this->Entity[3] = GameEntity(renderEngine->GetMesh("cube"), wallMaterial);
	this->Entity[3].ScaleTo(XMFLOAT3(1, 25, 50));
	this->Entity[3].TranslateTo(XMFLOAT3(-25, 10, 0));
	// wall 4
	this->Entity[4] = GameEntity(renderEngine->GetMesh("cube"), wallMaterial);
	this->Entity[4].ScaleTo(XMFLOAT3(1, 25, 50));
	this->Entity[4].TranslateTo(XMFLOAT3(25, 10, 0));

	// bunny model
	this->Entity[5] = GameEntity(multiPoly, meshMaterial);
	this->Entity[5].ScaleTo(XMFLOAT3(2, 2, 2));
	this->Entity[5].TranslateTo(XMFLOAT3(0, 0, 0));

	this->Entity[6] = GameEntity(multiPoly, meshMaterial);
	this->Entity[6].ScaleTo(XMFLOAT3(2, 2, 2));
	this->Entity[6].TranslateTo(XMFLOAT3(14, 0, 14));

	// Create Lights
	this->numPointLights = 1;
	this->PointLights = new ScenePointLight[numPointLights];
	this->PointLights[0] = ScenePointLight(XMFLOAT4(0.8, 0.8, 0.1, 1.0),
							XMFLOAT3(0.0, 1.0, 2.0),
							5);

	this->numDirLights = 1;
	this->DirLights = new SceneDirectionalLight[numDirLights];
	this->DirLights[0] = SceneDirectionalLight(
		XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT3(0, 5, -10));
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
	camera->mkProjMat((float) width, height);
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

	if (GetAsyncKeyState('W') & 0x8000)
		camera->Forward(amount);
	if (GetAsyncKeyState('S') & 0x8000)
		camera->Backward(amount);
	if (GetAsyncKeyState('A') & 0x8000)
		camera->StrafeLeft(amount);
	if (GetAsyncKeyState('D') & 0x8000)
		camera->StrafeRight(amount);

	/*
	if (GetAsyncKeyState(' ') & 0x8000 || GetAsyncKeyState('Q') & 0x8000)
		camera->MoveUp(amount);
	if (GetAsyncKeyState('X') & 0x8000 || GetAsyncKeyState('E') & 0x8000)
		camera->MoveDown(amount);
	*/

	// Render foveallly
	bool currTab = (GetAsyncKeyState('	') & 0x8000) != 0;
	if (currTab && !prevTab)
		renderFoveal = !renderFoveal;
	prevTab = currTab;

	// Pull from the mouse position or the eye tracker
	bool currShift = (GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0;
	if (currShift && !prevShift)
		useMouse = !useMouse;
	prevShift = currShift;

	// RESET THE CAMERA 
	if (GetAsyncKeyState('R') & 0x8000)
		camera->SetPos(DirectX::XMFLOAT3(0, 3, -10));
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	int curX, curY;
	if (useMouse)
	{
		curX = prevMousePos.x;
		curY = prevMousePos.y;
	}
	else {
		curX = eyeTracker->GetXPos();
		curY = eyeTracker->GetYPos();
	}

	renderEngine->FovealRender(curX, curY,
		Entity, numEntity,
		PointLights, numPointLights,
		DirLights, numDirLights, renderFoveal);

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