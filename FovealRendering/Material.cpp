#include "Material.h"


Material::Material()
{
	this->pixelShader = "default";
}

Material::Material(std::string pixelShader)
{
	this->pixelShader = pixelShader;
}

Material::Material(ID3D11SamplerState* sampler, ID3D11ShaderResourceView* textureSRV, ID3D11ShaderResourceView* normalSRV)
{
	this->pixelShader = "default";
	this->textureSRV = textureSRV;
	this->normalSRV = normalSRV;
	this->sampler = sampler;
}

Material::Material(std::string pixelShader, ID3D11SamplerState* sampler, ID3D11ShaderResourceView* textureSRV, ID3D11ShaderResourceView* normalSRV)
{
	this->pixelShader = pixelShader;
	this->textureSRV = textureSRV;
	this->normalSRV = normalSRV;
	this->sampler = sampler;
}


Material::~Material()
{
	if (sampler) sampler->Release();
	if (textureSRV) textureSRV->Release();
	if (normalSRV) normalSRV->Release();
}
