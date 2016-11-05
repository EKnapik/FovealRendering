#pragma once
#include "SimpleShader.h"
#include "WICTextureLoader.h"

class Material
{
public:
	Material();
	Material(std::string pixelShader);
	Material(ID3D11SamplerState* sampler, ID3D11ShaderResourceView* textureSRV, ID3D11ShaderResourceView* normalSRV);
	Material(std::string pixelShader, ID3D11SamplerState* sampler, ID3D11ShaderResourceView* textureSRV, ID3D11ShaderResourceView* normalSRV);
	~Material();

	std::string GetPixelShaderName() { return pixelShader; }
	ID3D11ShaderResourceView* GetTexture() { return textureSRV; }
	ID3D11ShaderResourceView* GetNormMap() { return normalSRV; }
	ID3D11SamplerState* GetSampler() { return sampler; }

private:
	std::string pixelShader;
	ID3D11ShaderResourceView* textureSRV = NULL;
	ID3D11ShaderResourceView* normalSRV = NULL;
	ID3D11SamplerState* sampler = NULL;
};

