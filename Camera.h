#pragma once
#include "Matrix.h"

class Camera
{
public:
	Camera(const float xRotation, const float yRotation, const float zRotation, const Vertex& position);
	~Camera();

	Vertex GetPosition();
	Matrix RetrieveCameraMatrix();

	void UpdateCameraMatrix();

private:
	void ChangeXRotation(const float xRotation);
	void ChangeYRotation(const float yRotation);
	void ChangeZRotation(const float zRotation);
	void ChangePosition(const Vertex& move);
	void GenerateCameraMatrix();

	float _xRotation;
	float _yRotation;
	float _zRotation;
	Vertex _position;
	Matrix _cameraMatrix;

};
