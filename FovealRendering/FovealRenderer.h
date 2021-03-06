#pragma once
#include "Renderer.h"


/* TODO:
   I believe that the final version should have a smaller texture size for the low
   resolution area that I sample then from a poorer texture and save on rendering time
   there aswell that.
*/
class FovealRenderer : public Renderer
{
public:
	FovealRenderer(Camera *camera, ID3D11Device* device, ID3D11DeviceContext *context,
		ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView, int width, int height);
	virtual ~FovealRenderer();

	void FovealRender(int eyePosX, int eyePosY, GameEntity *entities, int numEntities,
		ScenePointLight* pointLights, int numPointLights,
		SceneDirectionalLight* dirLights, int numDirLights, bool foveal);

private:
	void DrawLowRes(GameEntity *entities, int numEntities);
	void DrawMidRes(GameEntity *entities, int numEntities);
	void DrawHighRes(GameEntity *entities, int numEntities);

	void gBufferRender(int eyePosX, int eyePosY, GameEntity *entities, int numEntities, bool foveal);
	void DrawMask(int eyePosX, int eyePosY, float FovealOffset);
	void pointLightRender(ScenePointLight* pointLights, int numLights);
	void directionalLightRender(SceneDirectionalLight* dirLights, int numLights);
	void DrawMultipleMaterials(GameEntity* gameEntitys, int numEntities);

	int windowWidth;
	int windowHeight;

	// Albedo
	ID3D11RenderTargetView *	AlbedoRTV;
	ID3D11ShaderResourceView*	AlbedoSRV;

	// Normal
	ID3D11RenderTargetView *	NormalRTV;
	ID3D11ShaderResourceView*	NormalSRV;

	// Depth
	ID3D11RenderTargetView *	PositionRTV;
	ID3D11ShaderResourceView*	PositionSRV;

	ID3D11SamplerState* simpleSampler;
	ID3D11BlendState* blendState;
	ID3D11RasterizerState* lightRastState;
	ID3D11DepthStencilState* writeMask;
	ID3D11DepthStencilState* readMask;
};

