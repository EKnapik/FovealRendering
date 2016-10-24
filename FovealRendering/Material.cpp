#include "Material.h"



Material::Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader)
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
}

Material::Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11SamplerState* sampler,
	ID3D11ShaderResourceView* textureSRV, ID3D11ShaderResourceView* normalSRV)
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
	this->textureSRV = textureSRV;
	this->normalSRV = normalSRV;
	this->sampler = sampler;
}


Material::~Material()
{
	if (vertexShader)
		delete vertexShader;
	if (pixelShader)
		delete pixelShader;
	sampler->Release();
	textureSRV->Release();
	normalSRV->Release();
}
