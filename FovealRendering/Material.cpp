#include "Material.h"


Material::Material()
{
	this->pixelShader = "default";
}

Material::Material(std::string pixelShader)
{
	this->pixelShader = pixelShader;
}

Material::Material(ID3D11SamplerState* sampler, ID3D11ShaderResourceView* textureSRV)
{
	this->pixelShader = "default";
	this->textureSRV = textureSRV;
	this->sampler = sampler;
}

Material::Material(std::string pixelShader, ID3D11SamplerState* sampler, ID3D11ShaderResourceView* textureSRV)
{
	this->pixelShader = pixelShader;
	this->textureSRV = textureSRV;
	this->sampler = sampler;
}


Material::~Material()
{
	if (textureSRV) textureSRV->Release();
}
