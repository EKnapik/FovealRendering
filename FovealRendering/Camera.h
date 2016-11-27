#pragma once
#include <DirectXMath.h>

#define CAM_DIR XMFLOAT3(0, 0, 1)
#define CAM_UP XMFLOAT3(0, 1, 0)

/*
Look to camera implementation
THIS CAMERA DOES NOT CHANGE Y.
*/
class Camera
{
public:
	Camera();
	~Camera();

	DirectX::XMFLOAT3* GetDir();
	DirectX::XMFLOAT3* GetPos() { return &pos; }
	float GetXRot() { return xRot; }
	float GetYRot() { return yRot; }

	DirectX::XMFLOAT4X4* GetViewMat();
	DirectX::XMFLOAT4X4* GetProjMat() { return &projMat; }
	void mkProjMat(int width, int height);

	void SetPos(DirectX::XMFLOAT3 newPos);
	void Forward(float amount);
	void Backward(float amount);
	void StrafeRight(float amount);
	void StrafeLeft(float amount);
	// void MoveUp(float amount) { pos.y = pos.y + amount; }
	// void MoveDown(float amount) { MoveUp(-amount); }

	void AddXRot(float amount) { xRot = xRot + amount; }
	void SubXRot(float amount) { AddXRot(-amount); }
	void AddYRot(float amount) { yRot = yRot + amount; }
	void SubYRot(float amount) { AddYRot(-amount); }

private:
	DirectX::XMFLOAT3 pos;
	float xRot;
	float yRot;

	DirectX::XMFLOAT4X4 projMat;
};

