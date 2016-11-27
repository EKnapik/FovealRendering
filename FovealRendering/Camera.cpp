#include "Camera.h"

using namespace DirectX;

/*
THIS CAMERA DOES NOT CHANGE Y
*/
Camera::Camera()
{
	pos = XMFLOAT3(0, 0, 0);
	xRot = 6.28318530718;
	yRot = 0;
}


Camera::~Camera()
{
}


// gets the current camera direction
// normaized direction of the camera
DirectX::XMFLOAT3* Camera::GetDir() {
	XMFLOAT3 dir = CAM_DIR;
	XMVECTOR curDir = XMLoadFloat3(&dir);
	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(xRot, yRot, 0);
	curDir = XMVector3Normalize(XMVector3Transform(curDir, rotMat));
	XMStoreFloat3(&dir, curDir);

	return &dir;
}


// could use the GetDir function but I would be creating
// the rotation matrix twice, once for up vector and once for direction
//
// Returns a column ordering view matrix of this camera
DirectX::XMFLOAT4X4* Camera::GetViewMat() {
	XMFLOAT3 dir = CAM_DIR;
	XMFLOAT3 up = CAM_UP;
	XMFLOAT4X4 viewMat;
	XMVECTOR curPos = XMLoadFloat3(&pos);
	XMVECTOR curDir = XMLoadFloat3(&dir);
	XMVECTOR curUp = XMLoadFloat3(&up);
	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(xRot, yRot, 0);
	curDir = XMVector3Transform(curDir, rotMat);
	curUp = XMVector3Transform(curUp, rotMat);

	// saves the transposed into column ordering view matrix
	XMStoreFloat4x4(&viewMat, XMMatrixTranspose(
						XMMatrixLookToLH(curPos, curDir, curUp)));
	return &viewMat;
}


void Camera::mkProjMat(int width, int height) {
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projMat, XMMatrixTranspose(P)); // Transpose for HLSL!
}


void Camera::SetPos(XMFLOAT3 newPos)
{
	pos = newPos;
}

void Camera::Forward(float amount) {
	XMFLOAT3 dir = *GetDir();
	pos.x = pos.x + (dir.x*amount);
	// pos.y = pos.y + (dir.y*amount);
	pos.z = pos.z + (dir.z*amount);
}

void Camera::Backward(float amount) {
	Forward(-amount);
}


void Camera::StrafeRight(float amount) {
	XMFLOAT3 right;
	XMFLOAT3 dir = CAM_DIR;
	XMFLOAT3 up = CAM_UP;
	XMVECTOR curRight;
	XMVECTOR curDir = XMLoadFloat3(&dir);
	XMVECTOR curUp = XMLoadFloat3(&up);
	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(xRot, yRot, 0);
	curDir = XMVector3Transform(curDir, rotMat);
	curUp = XMVector3Transform(curUp, rotMat);

	curRight = XMVector3Normalize(XMVector3Cross(curUp, curDir));
	curRight = curRight * amount;
	XMStoreFloat3(&right, curRight);

	pos.x = pos.x + right.x;
	// pos.y = pos.y + right.y;
	pos.z = pos.z + right.z;
}


void Camera::StrafeLeft(float amount) {
	StrafeRight(-amount);
}
