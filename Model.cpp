#include "Model.h"
#include <algorithm>

bool isGreater(Polygon3D x, Polygon3D y)
{
	return x.GetAverageZ() > y.GetAverageZ();
}

Model::Model()
{
	_polygons = vector<Polygon3D>();
	_vertices = vector<Vertex>();
	
	ka_red = 0.2f;
	ka_green = 0.2f;
	ka_blue = 0.2f;

	kd_red = 0.5f;
	kd_green= 0.5f;
	kd_blue = 0.5f;
	
	ks_red = 1.0f;
	ks_green = 1.0f;
	ks_blue = 1.0f;

	_roughness = 30.0f;
}

Model::~Model()
{
}

void Model::Clear()
{
	_vertices.clear();
	_polygons.clear();
	_transformedVertices.clear();
	_uvCoordinates.clear();
}

const vector<Vertex>& Model::GetVertices()
{
	return _vertices;
}

const vector<Vertex>& Model::GetTransformedVertices()
{
	return _transformedVertices;
}

const vector<Polygon3D>& Model::GetPolygons()
{
	return _polygons;
}

const vector<TextureUV>& Model::GetUV()
{
	return _uvCoordinates;
}

float Model::GetAmbientRedCoefficient() const
{
	return ka_red;
}

float Model::GetAmbientGreenCoefficient() const
{
	return ka_green;
}

float Model::GetAmbientBlueCoefficient() const
{
	return ka_blue;
}

float Model::GetDiffuseRedCoefficient() const
{
	return kd_red;
}

float Model::GetDiffuseGreenCoefficient() const
{
	return kd_green;
}

float Model::GetDiffuseBlueCoefficient() const
{
	return kd_blue;
}

float Model::GetSpecularRedCoefficient() const
{
	return ks_red;
}

float Model::GetSpecularGreenCoefficient() const
{
	return ks_green;
}

float Model::GetSpecularBlueCoefficient() const
{
	return ks_blue;
}

float Model::GetRoughness() const
{
	return _roughness;
}

Texture& Model::GetTexture()
{
	return _texture;
}

size_t Model::GetVertexCount() const
{
	return _vertices.size();
}

size_t Model::GetTransformedVertexCount() const
{
	return _transformedVertices.size();
}

size_t Model::GetPolygonCount() const
{
	return _polygons.size();
}

void Model::AddVertex(float x, float y, float z)
{
	_vertices.push_back(Vertex(x, y, z));
}

void Model::AddPolygon(int i0, int i1, int i2, int uvIndex0, int uvIndex1, int uvIndex2)
{
	_polygons.push_back(Polygon3D(i0, i1, i2, uvIndex0, uvIndex1, uvIndex2));
}

void Model::AddTextureUV(float u, float v)
{
	_uvCoordinates.push_back(TextureUV(u, v));
}

void Model::ApplyTransformToLocalVertices(const Matrix& transform)
{
	_transformedVertices.clear();

	for (size_t i = 0; i < GetVertexCount(); i++)
	{
		_transformedVertices.push_back(transform * _vertices[i]);
	}
}

void Model::ApplyTransformToTransformedVertices(const Matrix& transform)
{
	for (size_t i = 0; i < GetTransformedVertexCount(); i++)
	{
		_transformedVertices[i] = transform * _transformedVertices[i];
	}
}

void Model::DehomogenizeAll()
{
	for (size_t i = 0; i < GetTransformedVertexCount(); i++)
	{
		_transformedVertices[i].Dehomogenize();
	}
}

void Model::CalculateBackfaces(const Vertex& cameraPosition)
{
	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		Vertex vertex0 = _transformedVertices[_polygons[i].GetIndex(0)];
		Vertex vertex1 = _transformedVertices[_polygons[i].GetIndex(1)];
		Vertex vertex2 = _transformedVertices[_polygons[i].GetIndex(2)];

		_polygons[i].SetNormalVector(Vector3D::CrossProduct(vertex0.GenerateVector(vertex2), vertex0.GenerateVector(vertex1)));
		Vector3D eyeVector = cameraPosition.GenerateVector(vertex0);
		if (Vector3D::DotProduct(_polygons[i].GetNormalVector(), eyeVector) < 0)
		{
			_polygons[i].SetVisibility(false);
		}
		else
		{
			_polygons[i].SetVisibility(true);
		}
	}
}

void Model::CalculateLightingAmbient(const AmbientLight& light)
{
	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		int red = static_cast<int>(light.GetRedIntensity() * ka_red);
		int green = static_cast<int>(light.GetGreenIntensity() * ka_green);
		int blue = static_cast<int>(light.GetBlueIntensity() * ka_blue);

		_polygons[i].SetColor(red, green, blue);
	}
}

void Model::CalculateLightingDirectional(const vector<DirectionalLight>& lights)
{
	int totalR;
	int totalG;
	int totalB;
	int tempR;
	int tempG;
	int tempB;

	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		Polygon3D& polygon = _polygons[i];

		totalR = GetRValue(polygon.GetColor());
		totalG = GetGValue(polygon.GetColor());
		totalB = GetBValue(polygon.GetColor());

		for (const DirectionalLight& light : lights)
		{
			float dotProduct = Vector3D::DotProduct(light.GetSourceDirection().GetUnitVector(), polygon.GetNormalVector().GetUnitVector());
			if (dotProduct > 0)
			{
				tempR = light.GetRedIntensity();
				tempG = light.GetGreenIntensity();
				tempB = light.GetBlueIntensity();

				tempR = static_cast<int>(tempR * kd_red);
				tempG = static_cast<int>(tempG * kd_green);
				tempB = static_cast<int>(tempB * kd_blue);

				tempR = static_cast<int>(tempR * dotProduct);
				tempG = static_cast<int>(tempG * dotProduct);
				tempB = static_cast<int>(tempB * dotProduct);

				totalR += tempR;
				totalG += tempG;
				totalB += tempB;
			}
		}

		totalR = Clamp(0, 255, totalR);
		totalG = Clamp(0, 255, totalG);
		totalB = Clamp(0, 255, totalB);

		polygon.SetColor(totalR, totalG, totalB);
	}
}

void Model::CalculateLightingPoint(const vector<PointLight>& lights)
{
	int totalR;
	int totalG;
	int totalB;
	int tempR;
	int tempG;
	int tempB;

	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		Polygon3D& polygon = _polygons[i];

		totalR = GetRValue(polygon.GetColor());
		totalG = GetGValue(polygon.GetColor());
		totalB = GetBValue(polygon.GetColor());

		for (const PointLight& light : lights)
		{
			Vector3D lightVector = light.GetSourcePosition().GenerateVector(_transformedVertices[polygon.GetIndex(0)]);
			float dotProduct = Vector3D::DotProduct(lightVector.GetUnitVector(), polygon.GetNormalVector().GetUnitVector());

			if (dotProduct > 0)
			{
				tempR = light.GetRedIntensity();
				tempG = light.GetGreenIntensity();
				tempB = light.GetBlueIntensity();

				tempR = static_cast<int>(tempR * kd_red);
				tempG = static_cast<int>(tempG * kd_green);
				tempB = static_cast<int>(tempB * kd_blue);

				tempR = static_cast<int>(tempR * dotProduct);
				tempG = static_cast<int>(tempG * dotProduct);
				tempB = static_cast<int>(tempB * dotProduct);

				float d = lightVector.GetLength();
				float attenuation = 100.0f / (light.GetA() + light.GetB() * d + light.GetC() * d * d);
				tempR = static_cast<int>(tempR * attenuation);
				tempG = static_cast<int>(tempG * attenuation);
				tempB = static_cast<int>(tempB * attenuation);

				totalR += tempR;
				totalG += tempG;
				totalB += tempB;
			}
		}

		totalR = Clamp(0, 255, totalR);
		totalG = Clamp(0, 255, totalG);
		totalB = Clamp(0, 255, totalB);

		polygon.SetColor(totalR, totalG, totalB);
	}
}

void Model::CalculateLightingAmbientSmooth(const AmbientLight& light)
{
	for (size_t i = 0; i < GetTransformedVertexCount(); i++)
	{
		int red = static_cast<int>(light.GetRedIntensity() * ka_red);
		int green = static_cast<int>(light.GetGreenIntensity() * ka_green);
		int blue = static_cast<int>(light.GetBlueIntensity() * ka_blue);

		_transformedVertices[i].SetColor(red, green, blue);
	}
}

void Model::CalculateLightingDirectionalSmooth(const vector<DirectionalLight>& lights)
{
	int totalR;
	int totalG;
	int totalB;
	int tempR;
	int tempG;
	int tempB;

	for (size_t i = 0; i < GetTransformedVertexCount(); i++)
	{
		Vertex& vertex = _transformedVertices[i];

		totalR = GetRValue(vertex.GetColor());
		totalG = GetGValue(vertex.GetColor());
		totalB = GetBValue(vertex.GetColor());

		for (const DirectionalLight& light : lights)
		{
			float dotProduct = Vector3D::DotProduct(light.GetSourceDirection().GetUnitVector(), vertex.GetNormalVector().GetUnitVector());
			if (dotProduct > 0)
			{
				tempR = light.GetRedIntensity();
				tempG = light.GetGreenIntensity();
				tempB = light.GetBlueIntensity();

				tempR = static_cast<int>(tempR * kd_red);
				tempG = static_cast<int>(tempG * kd_green);
				tempB = static_cast<int>(tempB * kd_blue);

				tempR = static_cast<int>(tempR * dotProduct);
				tempG = static_cast<int>(tempG * dotProduct);
				tempB = static_cast<int>(tempB * dotProduct);

				totalR += tempR;
				totalG += tempG;
				totalB += tempB;
			}
		}

		totalR = Clamp(0, 255, totalR);
		totalG = Clamp(0, 255, totalG);
		totalB = Clamp(0, 255, totalB);

		vertex.SetColor(totalR, totalG, totalB);
	}
}

void Model::CalculateLightingPointSmooth(const vector<PointLight>& lights)
{
	int totalR;
	int totalG;
	int totalB;
	int tempR;
	int tempG;
	int tempB;

	for (size_t i = 0; i < GetTransformedVertexCount(); i++)
	{
		Vertex& vertex = _transformedVertices[i];

		totalR = GetRValue(vertex.GetColor());
		totalG = GetGValue(vertex.GetColor());
		totalB = GetBValue(vertex.GetColor());

		for (const PointLight& light : lights)
		{
			Vector3D lightVector = light.GetSourcePosition().GenerateVector(vertex);
			float dotProduct = Vector3D::DotProduct(lightVector.GetUnitVector(), vertex.GetNormalVector().GetUnitVector());

			if (dotProduct > 0)
			{
				tempR = light.GetRedIntensity();
				tempG = light.GetGreenIntensity();
				tempB = light.GetBlueIntensity();

				tempR = static_cast<int>(tempR * kd_red);
				tempG = static_cast<int>(tempG * kd_green);
				tempB = static_cast<int>(tempB * kd_blue);

				tempR = static_cast<int>(tempR * dotProduct);
				tempG = static_cast<int>(tempG * dotProduct);
				tempB = static_cast<int>(tempB * dotProduct);

				float d = lightVector.GetLength();
				float attenuation = 100.0f / (light.GetA() + light.GetB() * d + light.GetC() * d * d);
				tempR = static_cast<int>(tempR * attenuation);
				tempG = static_cast<int>(tempG * attenuation);
				tempB = static_cast<int>(tempB * attenuation);

				totalR += tempR;
				totalG += tempG;
				totalB += tempB;
			}
		}

		totalR = Clamp(0, 255, totalR);
		totalG = Clamp(0, 255, totalG);
		totalB = Clamp(0, 255, totalB);

		vertex.SetColor(totalR, totalG, totalB);
	}
}

void Model::CalculateLightingDirectionalSmoothWithSpecular(const vector<DirectionalLight>& lights, const Vertex& cameraPosition)
{
	int totalR;
	int totalG;
	int totalB;

	for (size_t i = 0; i < GetTransformedVertexCount(); i++)
	{
		Vertex& vertex = _transformedVertices[i];

		totalR = GetRValue(vertex.GetColor());
		totalG = GetGValue(vertex.GetColor());
		totalB = GetBValue(vertex.GetColor());

		for (const DirectionalLight& light : lights)
		{
			Vector3D lightVector = light.GetSourceDirection();
			Vector3D vertexNormal = vertex.GetNormalVector();
			float dotProductDiffuse = Vector3D::DotProduct(lightVector.GetUnitVector(), vertexNormal.GetUnitVector());

			if (dotProductDiffuse > 0)
			{
				float diffuseRed = light.GetRedIntensity() * kd_red * dotProductDiffuse;
				float diffuseGreen = light.GetGreenIntensity() * kd_green * dotProductDiffuse;
				float diffuseBlue = light.GetBlueIntensity() * kd_blue * dotProductDiffuse;

				Vector3D reflection = vertexNormal.GetUnitVector() * 2 * Vector3D::DotProduct(lightVector, vertexNormal.GetUnitVector()) - lightVector;
				Vector3D view = cameraPosition.GenerateVector(vertex);
				float dotProductSpecular = max(0.0f, Vector3D::DotProduct(reflection.GetUnitVector(), view.GetUnitVector()));

				float specularRed = light.GetRedIntensity() * ks_red * powf(dotProductSpecular, _roughness);
				float specularGreen = light.GetGreenIntensity() * ks_green * powf(dotProductSpecular, _roughness);
				float specularBlue = light.GetBlueIntensity() * ks_blue * powf(dotProductSpecular, _roughness);
				
				totalR += static_cast<int>(diffuseRed + specularRed);
				totalG += static_cast<int>(diffuseGreen + specularGreen);
				totalB += static_cast<int>(diffuseBlue + specularBlue);
			}
		}

		totalR = Clamp(0, 255, totalR);
		totalG = Clamp(0, 255, totalG);
		totalB = Clamp(0, 255, totalB);

		vertex.SetColor(totalR, totalG, totalB);
	}
}

void Model::CalculateLightingPointSmoothWithSpecular(const vector<PointLight>& lights, const Vertex& cameraPosition)
{
	int totalR;
	int totalG;
	int totalB;

	for (size_t i = 0; i < GetTransformedVertexCount(); i++)
	{
		Vertex& vertex = _transformedVertices[i];

		totalR = GetRValue(vertex.GetColor());
		totalG = GetGValue(vertex.GetColor());
		totalB = GetBValue(vertex.GetColor());

		for (const PointLight& light : lights)
		{
			Vector3D lightVector = light.GetSourcePosition().GenerateVector(vertex);
			Vector3D vertexNormal = vertex.GetNormalVector();
			float dotProductDiffuse = Vector3D::DotProduct(lightVector.GetUnitVector(), vertex.GetNormalVector().GetUnitVector());

			if (dotProductDiffuse > 0)
			{
				float d = lightVector.GetLength();
				float attenuation = 100.0f / (light.GetA() + light.GetB() * d + light.GetC() * d * d);

				float diffuseRed = light.GetRedIntensity() * kd_red * dotProductDiffuse * attenuation;
				float diffuseGreen = light.GetGreenIntensity() * kd_green * dotProductDiffuse * attenuation;
				float diffuseBlue = light.GetBlueIntensity() * kd_blue * dotProductDiffuse * attenuation;

				Vector3D reflection = vertexNormal.GetUnitVector() * 2 * Vector3D::DotProduct(lightVector, vertexNormal.GetUnitVector()) - lightVector;
				Vector3D view = cameraPosition.GenerateVector(vertex);
				float dotProductSpecular = max(0.0f, Vector3D::DotProduct(reflection.GetUnitVector(), view.GetUnitVector()));

				float specularRed = light.GetRedIntensity() * ks_red * powf(dotProductSpecular, _roughness) * attenuation;
				float specularGreen = light.GetGreenIntensity() * ks_green * powf(dotProductSpecular, _roughness) * attenuation;
				float specularBlue = light.GetBlueIntensity() * ks_blue * powf(dotProductSpecular, _roughness) * attenuation;

				totalR += static_cast<int>(diffuseRed + specularRed);
				totalG += static_cast<int>(diffuseGreen + specularGreen);
				totalB += static_cast<int>(diffuseBlue + specularBlue);
			}
		}

		totalR = Clamp(0, 255, totalR);
		totalG = Clamp(0, 255, totalG);
		totalB = Clamp(0, 255, totalB);

		vertex.SetColor(totalR, totalG, totalB);
	}
}

float smoothstep(float edge0, float edge1, float x)
{
	float result;
	if (x < edge0)
	{
		result = 0.0f;
	}
	else if (x >= edge1)
	{
		result = 1.0f;
	}
	else
	{
		float t = (x - edge0) / (edge1 - edge0);
		result = (3.0f - 2.0f * t) * (t * t);
	}
	return result;
}

void Model::CalculateLightingSpotSmoothWithSpecular(const vector<SpotLight>& lights, const Vertex& cameraPosition)
{
	int totalR;
	int totalG;
	int totalB;

	for (size_t i = 0; i < GetTransformedVertexCount(); i++)
	{
		Vertex& vertex = _transformedVertices[i];

		totalR = GetRValue(vertex.GetColor());
		totalG = GetGValue(vertex.GetColor());
		totalB = GetBValue(vertex.GetColor());

		for (const SpotLight& light : lights)
		{
			Vector3D lightVector = light.GetSourcePosition().GenerateVector(vertex);
			Vector3D vertexNormal = vertex.GetNormalVector();
			float dotProductDiffuse = Vector3D::DotProduct(lightVector.GetUnitVector(), vertex.GetNormalVector().GetUnitVector());

			if (dotProductDiffuse > 0)
			{
				float d = lightVector.GetLength();
				float attenuation = 100.0f / (light.GetA() + light.GetB() * d + light.GetC() * d * d);
				
				float angle = Vector3D::DotProduct(light.GetSourcePosition().GenerateVector(vertex).GetUnitVector(), light.GetDirection().GetUnitVector());
				float spotLightMagic = smoothstep(light.GetOuterEdge(), light.GetInnerEdge(), angle);

				float diffuseRed = light.GetRedIntensity() * kd_red * dotProductDiffuse * attenuation * spotLightMagic;
				float diffuseGreen = light.GetGreenIntensity() * kd_green * dotProductDiffuse * attenuation * spotLightMagic;
				float diffuseBlue = light.GetBlueIntensity() * kd_blue * dotProductDiffuse * attenuation * spotLightMagic;

				Vector3D reflection = vertexNormal.GetUnitVector() * 2 * Vector3D::DotProduct(lightVector, vertexNormal.GetUnitVector()) - lightVector;
				Vector3D view = cameraPosition.GenerateVector(vertex);
				float dotProductSpecular = max(0.0f, Vector3D::DotProduct(reflection.GetUnitVector(), view.GetUnitVector()));

				float specularRed = light.GetRedIntensity() * ks_red * powf(dotProductSpecular, _roughness) * attenuation * spotLightMagic;
				float specularGreen = light.GetGreenIntensity() * ks_green * powf(dotProductSpecular, _roughness) * attenuation * spotLightMagic;
				float specularBlue = light.GetBlueIntensity() * ks_blue * powf(dotProductSpecular, _roughness) * attenuation * spotLightMagic;
				
				totalR += static_cast<int>(diffuseRed + specularRed);
				totalG += static_cast<int>(diffuseGreen + specularGreen);
				totalB += static_cast<int>(diffuseBlue + specularBlue);
			}
		}

		totalR = Clamp(0, 255, totalR);
		totalG = Clamp(0, 255, totalG);
		totalB = Clamp(0, 255, totalB);

		vertex.SetColor(totalR, totalG, totalB);
	}
}

void Model::Sort()
{
	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		Vertex vertex0 = _transformedVertices[_polygons[i].GetIndex(0)];
		Vertex vertex1 = _transformedVertices[_polygons[i].GetIndex(1)];
		Vertex vertex2 = _transformedVertices[_polygons[i].GetIndex(2)];

		_polygons[i].SetAverageZ((vertex0.GetZ() + vertex1.GetZ() + vertex2.GetZ()) / 3);
	}
	
	sort(_polygons.begin(), _polygons.end(), isGreater);
}

void Model::CalculateNormalVectorForVertices()
{
	for (size_t i = 0; i < GetTransformedVertexCount(); i++)
	{
		Vertex& vertex = _transformedVertices[i];

		vertex.SetNormalVector(Vector3D(0, 0, 0));
		vertex.SetContributions(0);
	}

	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		const Polygon3D& polygon = _polygons[i];

		Vertex& vertex0 = _transformedVertices[polygon.GetIndex(0)];
		Vertex& vertex1 = _transformedVertices[polygon.GetIndex(1)];
		Vertex& vertex2 = _transformedVertices[polygon.GetIndex(2)];

		vertex0.SetNormalVector(vertex0.GetNormalVector() + polygon.GetNormalVector());
		vertex1.SetNormalVector(vertex1.GetNormalVector() + polygon.GetNormalVector());
		vertex2.SetNormalVector(vertex2.GetNormalVector() + polygon.GetNormalVector());

		vertex0.SetContributions(vertex0.GetContributions() + 1);
		vertex1.SetContributions(vertex1.GetContributions() + 1);
		vertex2.SetContributions(vertex2.GetContributions() + 1);
	}

	for (size_t i = 0; i < GetTransformedVertexCount(); i++)
	{
		Vertex& vertex = _transformedVertices[i];

		vertex.SetNormalVector((vertex.GetNormalVector() / vertex.GetContributions()).GetUnitVector());
	}
}

int Model::Clamp(const int lower, const int upper, const int value)
{
	return max(lower, min(value, upper));
}
