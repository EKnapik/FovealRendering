#pragma once

#include <DirectXMath.h>

struct DirectionalLight
{
	DirectX::XMFLOAT4 AmbientColor;	    // The position of the vertex
	DirectX::XMFLOAT4 DiffuseColor;
	DirectX::XMFLOAT3 Direction;
};


struct PointLight
{
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Position;
	float Radius;
};

class SceneDirectionalLight {
public:
	SceneDirectionalLight() {}
	SceneDirectionalLight(DirectX::XMFLOAT4 aColor, DirectX::XMFLOAT4 dColor, DirectX::XMFLOAT3 direction);
	DirectX::XMFLOAT4 AmbientColor;
	DirectX::XMFLOAT4 DiffuseColor;
	DirectX::XMFLOAT3 Direction;
};

class ScenePointLight {
public:
	ScenePointLight() {}
	ScenePointLight(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 position, float radius);
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Radius;
};