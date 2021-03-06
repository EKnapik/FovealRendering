#pragma once
#include <DirectXMath.h>
#include <math.h>
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"



class GameEntity
{
public:
	GameEntity() {} // needed for array construction
	GameEntity(Mesh** meshes, Material* material);
	GameEntity(Mesh** meshes, DirectX::XMFLOAT3 pos, Material* material);
	GameEntity(Mesh* mesh, Material* material);
	GameEntity(Mesh* mesh, DirectX::XMFLOAT3 pos, Material* material);
	~GameEntity();

	Mesh* GetMesh() { return mesh; } // return the lowpoly version as default
	Mesh* GetMidPoly();
	Mesh* GetHighPoly();
	Material* GetMaterial() { return material; }
	bool IsDetailed() { return detailed; }

	void SetMesh(Mesh* mesh);

	// translates from the current position by:
	void TranslateBy(DirectX::XMFLOAT3 trans);
	void TranslateBy(float x, float y, float z);
	// translates to this current location
	void TranslateTo(DirectX::XMFLOAT3 newPos);
	void TranslateTo(float x, float y, float z);

	// moves along the current direction
	void MoveForward(float amount);

	// scales from the current scale by:
	void ScaleBy(DirectX::XMFLOAT3 scale);
	void ScaleBy(float x, float y, float z);
	void ScaleBy(float scale);
	// scales to this specific new scale
	void ScaleTo(DirectX::XMFLOAT3 scale);
	void ScaleTo(float x, float y, float z);
	void ScaleTo(float scale);

	// rotates by this rotation direction by some amount in each axis of rotation
	void RotateBy(DirectX::XMFLOAT3 dir);
	void RotateByX(float amount);
	void RotateByY(float amount);
	void RotateByZ(float amount);
	// rotates to this direction
	// rotates some amount about each axis of rotation
	void RotateTo(DirectX::XMFLOAT3 dir);
	void RotateTo(float x, float y, float z);

	// which poly level mesh to use as current mesh
	void WhichPoly(Camera* camera);

	void CreateWorldMat();
	DirectX::XMFLOAT4X4* GetWorld();
	DirectX::XMFLOAT4X4* GetWorldClean();
	DirectX::XMFLOAT4X4* GetRotationMat();

private:
	Mesh **meshes; // low, mid and high poly array
	Mesh* mesh; // current poly level mesh to use
	bool detailed;
	bool dirty;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 rotQtrn;
	// a column major world matrix
	DirectX::XMFLOAT4X4 world;

	Material* material;
};
