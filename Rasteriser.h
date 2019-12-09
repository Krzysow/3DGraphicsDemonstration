#pragma once
#include "Framework.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Model.h"
#include "MD2Loader.h"
#include "Camera.h"

class Rasteriser : public Framework
{
public:
	bool Initialise();
	void Update(const Bitmap& bitmap);
	void Render(const Bitmap& bitmap);

	Matrix Translate(const float translateX, const float translateY, const float translateZ);
	Matrix Scale(const float ScaleX, const float ScaleY, const float ScaleZ);
	Matrix RotateX(const float angle);
	Matrix RotateY(const float angle);
	Matrix RotateZ(const float angle);
	Matrix RotateXYZ(const float angleX, const float angleY, const float angleZ);

	void GeneratePerspectiveMatrix(float d, float aspectRatio);
	void GenerateViewMatrix(float d, int width, int height);

	void DrawWireFrame(const Bitmap& bitmap);
	void DrawSolidFlat(const Bitmap& bitmap);
	void MyDrawSolidFlat(const Bitmap& bitmap);

	void FillPolygonFlat(const HDC& hDc, const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2, const COLORREF& color);
	//void fillTopFlatTriangle(const HDC& hDc, const Vertex& v1, const Vertex& v2, const Vertex& v3, const COLORREF& color);
	//void fillBottomFlatTriangle(const HDC& hDc, const Vertex& v1, const Vertex& v2, const Vertex& v3, const COLORREF& color);

private:
	float _x{ 0.0f };
	float _y{ 0.0f };
	float _z{ 0.0f };

	Model _model;

	Matrix _modelMatrix;
	Camera _camera{ 0.0f, 0.0f, 0.0f, Vertex{ 0.0f, 0.0f, -50.0f } };
	Matrix _perspectiveMatrix;
	Matrix _viewMatrix;
	AmbientLight _ambientLight{ 200, 100, 50 };
	vector<DirectionalLight> _directionalLights;
	vector<PointLight> _pointLights;
};
