#include "Camera.h"
#include "cmath"

Camera::Camera(float xRotation, float yRotation, float zRotation, const Vertex& position)
{
	_xRotation = xRotation;
	_yRotation = yRotation;
	_zRotation = zRotation;
	_position = position;
	GenerateCameraMatrix();
}

Camera::~Camera()
{
}

Vertex Camera::GetPosition()
{
	return _position;
}

Matrix Camera::RetrieveCameraMatrix()
{
	return _cameraMatrix;
}

void Camera::GenerateCameraMatrix()
{
	float radians = Matrix::toRadians(_xRotation);
	Matrix rotateX{ 1, 0, 0, 0,
					0, cos(radians), sin(radians), 0,
					0, -sin(radians), cos(radians), 0,
					0, 0, 0, 1 };

	radians = Matrix::toRadians(_yRotation);
	Matrix rotateY{ cos(radians), 0, -sin(radians), 0,
					0, 1, 0, 0,
					sin(radians), 0, cos(radians), 0,
					0, 0, 0, 1 };

	radians = Matrix::toRadians(_zRotation);
	Matrix rotateZ{ cos(radians), sin(radians), 0, 0,
					-sin(radians), cos(radians), 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 };

	Matrix translate{ 1, 0, 0, -_position.GetX(),
					0, 1, 0, -_position.GetY(),
					0, 0, 1, -_position.GetZ(),
					0, 0, 0, 1 };

	_cameraMatrix = translate * rotateZ * rotateY * rotateX;
}

void Camera::UpdateCameraMatrix()
{
	ChangeXRotation(_xRotation);
	ChangeYRotation(_yRotation);
	ChangeZRotation(_zRotation);
	ChangePosition(_position);
	GenerateCameraMatrix();
}

void Camera::ChangeXRotation(const float xRotation)
{
	_xRotation += xRotation;
}

void Camera::ChangeYRotation(const float yRotation)
{
	_yRotation += yRotation;
}

void Camera::ChangeZRotation(const float zRotation)
{
	_zRotation += zRotation;
}

void Camera::ChangePosition(const Vertex& move)
{
	_position = _position + move;
}
