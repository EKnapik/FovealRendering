#pragma once
#include "SimpleShader.h"
#include "WICTextureLoader.h"

class Material
{
public:
	Material();
	Material(std::string pixelShader);
	Material(ID3D11SamplerState* sampler, ID3D11ShaderResourceView* textureSRV);
	Material(std::string pixelShader, ID3D11SamplerState* sampler, ID3D11ShaderResourceView* textureSRV);
	~Material();

	std::string GetPixelShaderName() { return pixelShader; }
	ID3D11ShaderResourceView* GetTexture() { return textureSRV; }
	ID3D11SamplerState* GetSampler() { return sampler; }

private:
	std::string pixelShader;
	ID3D11ShaderResourceView* textureSRV = NULL;
	ID3D11SamplerState* sampler = NULL;
};

