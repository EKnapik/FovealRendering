#include "Lights.h"

SceneDirectionalLight::SceneDirectionalLight(DirectX::XMFLOAT4 aColor, DirectX::XMFLOAT4 dColor, DirectX::XMFLOAT3 direction)
{
	AmbientColor = aColor;
	DiffuseColor = dColor;
	Direction = direction;
}

ScenePointLight::ScenePointLight(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 position, float radius)
{
	Color = color;
	Position = position;
	Radius = DirectX::XMFLOAT3(radius, radius, radius);
}
