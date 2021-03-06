#include "GameEntity.h"

using namespace DirectX;

// with low mid and high poly level meshes available
GameEntity::GameEntity(Mesh **meshes, Material* material)
{
	this->detailed = true;
	this->meshes = meshes;
	this->mesh = meshes[0];
	this->material = material;
	this->dirty = true;
	this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	RotateTo(0.0f, 0.0f, 0.0f); // sets the rotation quaterion
	this->world = XMFLOAT4X4();
}

GameEntity::GameEntity(Mesh **meshes, DirectX::XMFLOAT3 pos, Material* material)
{
	this->detailed = true;
	this->meshes = meshes;
	this->mesh = meshes[0];
	this->material = material;
	this->dirty = true;
	this->pos = pos;
	this->scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	RotateTo(0.0f, 0.0f, 0.0f); // sets the rotation quaterion
	this->world = XMFLOAT4X4();
}

// just single mesh for game entity
GameEntity::GameEntity(Mesh* mesh, Material* material)
{
	this->detailed = false;
	this->mesh = mesh;
	this->meshes = nullptr;
	this->material = material;
	this->dirty = true;
	this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	RotateTo(0.0f, 0.0f, 0.0f); // sets the rotation quaterion
	this->world = XMFLOAT4X4();
}

GameEntity::GameEntity(Mesh* mesh, DirectX::XMFLOAT3 pos, Material* material)
{
	this->detailed = false;
	this->mesh = mesh;
	this->meshes = nullptr;
	this->material = material;
	this->dirty = true;
	this->pos = pos;
	this->scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	RotateTo(0.0f, 0.0f, 0.0f); // sets the rotation quaterion
	this->world = XMFLOAT4X4();
}

GameEntity::~GameEntity()
{
}

Mesh * GameEntity::GetMidPoly()
{
	if (detailed)
	{
		return meshes[1];
	}
	return GetMesh();
}

Mesh * GameEntity::GetHighPoly()
{
	if (detailed)
	{
		return meshes[2];
	}
	return GetMesh();
}

void GameEntity::SetMesh(Mesh* mesh) {
	this->mesh = mesh;
}

void GameEntity::TranslateBy(DirectX::XMFLOAT3 trans)
{
	this->dirty = true;
	this->pos.x += trans.x;
	this->pos.y += trans.y;
	this->pos.z += trans.z;
}

void GameEntity::TranslateBy(float x, float y, float z)
{
	TranslateBy(XMFLOAT3(x, y, z));
}

void GameEntity::TranslateTo(DirectX::XMFLOAT3 newPos)
{
	this->dirty = true;
	this->pos = newPos;
}

void GameEntity::TranslateTo(float x, float y, float z)
{
	TranslateTo(XMFLOAT3(x, y, z));
}

/*
converts rotationVec into a direction vector
then moves along that vector by the amount
*/
void GameEntity::MoveForward(float amount)
{
	double pi = 3.14159265;
	// newPos = oldPos + dir*amount;
	// converts rotation
	
	XMFLOAT4 baseDir(0.0f, 0.0f, 1.0f, 1.0);
	XMVECTOR dir = XMLoadFloat4(&baseDir);
	XMVECTOR quat = XMLoadFloat4(&this->rotQtrn);
	quat = XMQuaternionNormalize(quat);
	XMMATRIX rotMat = XMMatrixRotationQuaternion(quat);
	dir = XMVector4Transform(dir, rotMat);
	dir = XMVector4Normalize(dir);
	dir = dir * amount;
	XMStoreFloat4(&baseDir, dir);

	TranslateBy(baseDir.x, baseDir.y, baseDir.z);
}

void GameEntity::ScaleBy(DirectX::XMFLOAT3 scale)
{
	this->dirty = true;
	this->scale.x *= scale.x;
	this->scale.y *= scale.y;
	this->scale.z *= scale.z;
}

void GameEntity::ScaleBy(float x, float y, float z)
{
	ScaleBy(XMFLOAT3(x, y, z));
}

void GameEntity::ScaleBy(float scale)
{
	ScaleBy(XMFLOAT3(scale, scale, scale));
}

void GameEntity::ScaleTo(DirectX::XMFLOAT3 scale)
{
	this->dirty = true;
	this->scale = scale;
}

void GameEntity::ScaleTo(float x, float y, float z)
{
	ScaleTo(XMFLOAT3(x, y, z));
}

void GameEntity::ScaleTo(float scale)
{
	ScaleTo(XMFLOAT3(scale, scale, scale));
}

void GameEntity::RotateBy(DirectX::XMFLOAT3 dir)
{
	RotateByX(dir.x);
	RotateByY(dir.y);
	RotateByZ(dir.z);
}

void GameEntity::RotateByX(float amount)
{
	this->dirty = true;
	XMVECTOR quat = XMLoadFloat4(&this->rotQtrn);
	XMStoreFloat4(&rotQtrn, 
		XMQuaternionMultiply(quat, XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), amount)));
}

void GameEntity::RotateByY(float amount)
{
	this->dirty = true;
	XMVECTOR quat = XMLoadFloat4(&this->rotQtrn);
	XMStoreFloat4(&rotQtrn,
		XMQuaternionMultiply(quat, XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), amount)));
}

void GameEntity::RotateByZ(float amount)
{
	this->dirty = true;
	XMVECTOR quat = XMLoadFloat4(&this->rotQtrn);
	XMStoreFloat4(&rotQtrn,
		XMQuaternionMultiply(quat, XMQuaternionRotationAxis(XMVectorSet(0, 0, 1, 0), amount)));
}

void GameEntity::RotateTo(DirectX::XMFLOAT3 dir)
{
	XMStoreFloat4(&rotQtrn, XMQuaternionIdentity());
	RotateBy(dir);
}

void GameEntity::RotateTo(float x, float y, float z)
{
	RotateTo(XMFLOAT3(x, y, z));
}

void GameEntity::WhichPoly(Camera* camera)
{
	if (!detailed) { return; }

	float lowRange;
	float midRange;

	// transform object to camera space and use to determine which poly level mesh to show
	XMFLOAT4X4 world = *GetWorld();
	//XMFLOAT4X4 world = camera->GetViewMat();
	DirectX::XMFLOAT3 camPos = *camera->GetPos();
	XMFLOAT4 pos4x4 = { pos.x, pos.y, pos.z, 1.0f };
	XMVECTOR vecPos4x4 = XMLoadFloat4(&pos4x4);
	XMMATRIX worldMatrix = XMLoadFloat4x4(&world);
	XMVECTOR polyCamSpaceVect = XMVector4Transform(vecPos4x4, worldMatrix);
	XMFLOAT4 polyCamSpace;
	XMStoreFloat4(&polyCamSpace, polyCamSpaceVect);
	//std::cout << "Object World: " << polyCamSpace.x << " " << polyCamSpace.y << " " << polyCamSpace.z << " " << "\n";
	//std::cout << "Object: " << pos.x << " " << pos.y << " " << pos.z << " " << "\n";
	//std::cout << "Camera: " << camPos.x << " " << camPos.y << " " << camPos.z << " " << "\n";
	float dist = sqrt(pow((camPos.x - polyCamSpace.x), 2) + pow((camPos.y - polyCamSpace.y), 2) + pow((camPos.z - polyCamSpace.z), 2));
	//std::cout << "Dist: " << dist <<"\n";

	lowRange = 13.0f;
	midRange = 10.0f;
	if (dist > lowRange) // low poly
	{
		this->mesh = meshes[0];
	}
	else if (dist > midRange && dist <= lowRange) // mid poly
	{
		this->mesh = meshes[1];
	}
	else // high poly
	{
		this->mesh = meshes[2];
	}
	
}

void GameEntity::CreateWorldMat()
{
	XMMATRIX trans = XMMatrixTranslation(pos.x, pos.y, pos.z);
	XMVECTOR quaternion = XMLoadFloat4(&this->rotQtrn);
	XMMATRIX rot = XMMatrixRotationQuaternion(quaternion);
	XMMATRIX scale = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
	XMMATRIX world = scale * rot * trans;

	XMStoreFloat4x4(&this->world, XMMatrixTranspose(world));
	this->dirty = false;
}

DirectX::XMFLOAT4X4* GameEntity::GetWorld()
{
	return &this->world;
}

DirectX::XMFLOAT4X4* GameEntity::GetWorldClean()
{
	if (dirty) {
		CreateWorldMat();
	}
	return GetWorld();
}

DirectX::XMFLOAT4X4* GameEntity::GetRotationMat()
{
	XMFLOAT4X4 returnMat;
	XMVECTOR quaternion = XMLoadFloat4(&this->rotQtrn);
	XMStoreFloat4x4(&returnMat,
		XMMatrixRotationQuaternion(quaternion));
	return &returnMat;
}




