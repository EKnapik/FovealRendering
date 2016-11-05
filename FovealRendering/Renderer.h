#pragma once
#include <map>
#include <string>
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
	Renderer(Camera *camera, ID3D11Device* device, ID3D11DeviceContext *context,
		ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView);
	virtual ~Renderer();

	// TODO implement this function
	void DrawFoveated(GameEntity *entities, int numEntities) {}
	void DrawOneMaterial(GameEntity *entities, int numEntities, float deltaTime, float totalTime);
	void DrawNoMaterial(GameEntity *entities, int numEntities, float deltaTime, float totalTime);

	void AddMesh(std::string name, Mesh* mesh);
	void AddMesh(std::string name, std::string path);
	Mesh* GetMesh(std::string name);

	void AddVertexShader(std::string name, std::wstring path);
	void AddPixelShader(std::string name, std::wstring path);

protected:
	Camera *camera;

	ID3D11Device*			device;
	ID3D11DeviceContext*	context;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;

	std::map<std::string, Mesh*>				MeshDictionary;
	std::map<std::string, SimpleVertexShader*>	VertexShaderDictionary;
	std::map<std::string, SimplePixelShader*>	PixelShaderDictionary;

	SimpleVertexShader* GetVertexShader(std::string name);
	SimplePixelShader* GetPixelShader(std::string name);
};

