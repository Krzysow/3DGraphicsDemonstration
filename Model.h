#pragma once
#include "Matrix.h"
#include "Matrix.h"
#include "Polygon3D.h"
#include "Camera.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include <vector>

using namespace std;

//bool isGreater(Polygon3D x, Polygon3D y);

class Model
{
public:
	Model();
	~Model();

	const vector<Vertex>& GetVertices();
	const vector<Vertex>& GetTransformedVertices();
	const vector<Polygon3D>& GetPolygons();

	size_t GetVertexCount() const;
	size_t GetTransformedVertexCount() const;
	size_t GetPolygonCount() const;

	const float GetRedAmbientCoefficient();
	const float GetGreenAmbientCoefficient();
	const float GetBlueAmbientCoefficient();

	const float GetRedDifusedCoefficient();
	const float GetGreenDifusedCoefficient();
	const float GetBlueDifusedCoefficient();

	void AddVertex(float x, float y, float z);
	void AddPolygon(int i0, int i1, int i2);

	void ApplyTransformToLocalVertices(const Matrix& transform);
	void ApplyTransformToTransformedVertices(const Matrix& transform);

	void DehomogenizeAll();

	void CalculateBackfaces(const Vertex& camera);

	void CalculateLightingAmbient(const AmbientLight& light);
	void CalculateLightingDirectional(const vector<DirectionalLight>& lights);
	void CalculateLightingPoint(const vector<PointLight>& lights);
	void CalculateLightingDirectionalSmooth(const vector<DirectionalLight>& lights);
	void CalculateLightingPointSmooth(const vector<PointLight>& lights);

	void Sort();

	void CalculateNormalVectorForVertices();

private:
	static int Clamp(const int min, const int max, const int value);

	vector<Vertex> _vertices;
	vector<Polygon3D> _polygons;
	vector<Vertex> _transformedVertices;
	float ka_red;
	float ka_green;
	float ka_blue;

	float kd_red;
	float kd_green;
	float kd_blue;

	float kp_red;
	float kp_green;
	float kp_blue;
};

