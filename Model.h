#pragma once
#include "Matrix.h"
#include "Polygon3D.h"
#include "Camera.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "TextureUV.h"
#include "Texture.h"
#include <vector>

using namespace std;

//bool isGreater(Polygon3D x, Polygon3D y);

class Model
{
public:
	Model();
	~Model();

	void Clear();

	const vector<Vertex>& GetVertices();
	const vector<Vertex>& GetTransformedVertices();
	const vector<Polygon3D>& GetPolygons();
	const vector<TextureUV>& GetUV();

	Texture& GetTexture();

	size_t GetVertexCount() const;
	size_t GetTransformedVertexCount() const;
	size_t GetPolygonCount() const;

	void AddVertex(float x, float y, float z);
	void AddPolygon(int i0, int i1, int i2, int uvIndex0, int uvIndex1, int uvIndex2);
	void AddTextureUV(float u, float v);

	void ApplyTransformToLocalVertices(const Matrix& transform);
	void ApplyTransformToTransformedVertices(const Matrix& transform);

	void DehomogenizeAll();

	void CalculateBackfaces(const Vertex& camera);

	void CalculateLightingAmbient(const AmbientLight& light);
	void CalculateLightingDirectional(const vector<DirectionalLight>& lights);
	void CalculateLightingPoint(const vector<PointLight>& lights);

	void CalculateLightingAmbientSmooth(const AmbientLight& light);
	void CalculateLightingDirectionalSmooth(const vector<DirectionalLight>& lights);
	void CalculateLightingPointSmooth(const vector<PointLight>& lights);

	void CalculateLightingDirectionalSmoothWithSpecular(const vector<DirectionalLight>& lights, const Vertex& cameraPosition);
	void CalculateLightingPointSmoothWithSpecular(const vector<PointLight>& lights, const Vertex& cameraPosition);
	void CalculateLightingSpotSmoothWithSpecular(const vector<SpotLight>& lights, const Vertex& cameraPosition);

	void Sort();

	void CalculateNormalVectorForVertices();

	static int Clamp(const int min, const int max, const int value);

private:
	vector<Vertex> _vertices;
	vector<Polygon3D> _polygons;
	vector<Vertex> _transformedVertices;
	vector<TextureUV> _uvCoordinates;
	
	float ka_red;
	float ka_green;
	float ka_blue;

	float kd_red;
	float kd_green;
	float kd_blue;

	float ks_red;
	float ks_green;
	float ks_blue;

	float _roughness;

	Texture _texture;
};
