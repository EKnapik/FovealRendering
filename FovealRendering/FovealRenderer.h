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
	~FovealRenderer();

	void Render(int eyePosX, int eyePosY);

private:
	void DrawLowRes();
	void DrawHighRes();
	void DrawFinal();

	int windowWidth;
	int windowHeight;

	Mesh* quad;
	SimpleVertexShader*		gBufferVertexShader;
	SimplePixelShader*		gBufferPixelShader;

	SimpleVertexShader*		quadVertexShader;
	SimplePixelShader*		quadPixelShader;
	SimplePixelShader*		fovealPixelShader;

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
};

