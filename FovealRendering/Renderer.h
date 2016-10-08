#pragma once
#include <DirectXMath.h>
#include "GameEntity.h"
#include "Camera.h"
#include "Lights.h"


/* This class manages and renders the objects in the most efficient way
it can.

*/
//TODO: Actually do efficient managing of the objects
class Renderer
{
public:
	Renderer(Camera *camera, ID3D11DeviceContext *context,
		ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView);
	~Renderer();

	void DrawOneMaterial(GameEntity *entities, int numEntities, float deltaTime, float totalTime);

private:
	Camera *camera;

	ID3D11DeviceContext*	context;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;
};

