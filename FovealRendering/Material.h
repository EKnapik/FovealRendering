#pragma once
#include "SimpleShader.h"
#include "WICTextureLoader.h"

class Material
{
public:
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader);
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11SamplerState* sampler,
		ID3D11ShaderResourceView* textureSRV, ID3D11ShaderResourceView* normalSRV);
	~Material();

	SimpleVertexShader* GetVertexShader() { return vertexShader; }
	SimplePixelShader* GetPixelShader() { return pixelShader; }
	ID3D11ShaderResourceView* GetTexture() { return textureSRV; }
	ID3D11ShaderResourceView* GetNormMap() { return normalSRV; }
	ID3D11SamplerState* GetSampler() { return sampler; }

private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	ID3D11ShaderResourceView* textureSRV = NULL;
	ID3D11ShaderResourceView* normalSRV = NULL;
	ID3D11SamplerState* sampler = NULL;
};

