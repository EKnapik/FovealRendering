#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include <map>
#include <string>
#include "Mesh.h"
#include "GameEntity.h"
#include "Camera.h"
#include "Lights.h"
#include "EyeTracker.h"
#include "FovealRenderer.h"


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

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();

	EyeTracker *eyeTracker;
	Camera *camera;
	FovealRenderer *renderEngine;

	Mesh** multiBunny;
	Mesh** multiDillo;

	GameEntity *Entity;
	int numEntity;

	ScenePointLight* PointLights;
	int numPointLights;

	SceneDirectionalLight* DirLights;
	int numDirLights;

	// Wrappers for DirectX shaders to provide simplified functionality
	ID3D11SamplerState* tmpSampler;
	Material* meshMaterial;
	Material* floorMaterial;
	Material* wallMaterial;
	Material* noMaterial;

	bool renderFoveal = true;
	bool prevTab = false;

	bool useMouse = true;
	bool prevShift = false;

	POINT prevMousePos;
};

