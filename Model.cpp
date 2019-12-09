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
}

Model::~Model()
{
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

const float Model::GetRedAmbientCoefficient()
{
	return ka_red;
}

const float Model::GetGreenAmbientCoefficient()
{
	return ka_green;
}

const float Model::GetBlueAmbientCoefficient()
{
	return ka_blue;
}

const float Model::GetRedDifusedCoefficient()
{
	return kd_red;
}

const float Model::GetGreenDifusedCoefficient()
{
	return kd_green;
}

const float Model::GetBlueDifusedCoefficient()
{
	return kd_blue;
}

void Model::AddVertex(float x, float y, float z)
{
	_vertices.push_back(Vertex(x, y, z));
}

void Model::AddPolygon(int i0, int i1, int i2)
{
	_polygons.push_back(Polygon3D(i0, i1, i2));
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
		totalR = GetRValue(_polygons[i].GetColor());
		totalG = GetGValue(_polygons[i].GetColor());
		totalB = GetBValue(_polygons[i].GetColor());

		for (const DirectionalLight& light : lights)
		{
			float dotProduct = Vector3D::DotProduct(light.GetSourceDirection().GetUnitVector(), _polygons[i].GetNormalVector().GetUnitVector());
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

		_polygons[i].SetColor(totalR, totalG, totalB);
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
		totalR = GetRValue(_polygons[i].GetColor());
		totalG = GetGValue(_polygons[i].GetColor());
		totalB = GetBValue(_polygons[i].GetColor());

		for (const PointLight& light : lights)
		{
			Vector3D lightVector = light.GetSourcePosition().GenerateVector(_transformedVertices[_polygons[i].GetIndex(0)]);
			float dotProduct = Vector3D::DotProduct(lightVector.GetUnitVector(), _polygons[i].GetNormalVector().GetUnitVector());

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

		_polygons[i].SetColor(totalR, totalG, totalB);
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
		totalR = GetRValue(_transformedVertices[i].GetColor());
		totalG = GetGValue(_transformedVertices[i].GetColor());
		totalB = GetBValue(_transformedVertices[i].GetColor());

		for (const DirectionalLight& light : lights)
		{
			float dotProduct = Vector3D::DotProduct(light.GetSourceDirection().GetUnitVector(), _transformedVertices[i].GetNormalVector().GetUnitVector());
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

		_transformedVertices[i].SetColor(totalR, totalG, totalB);
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
		totalR = GetRValue(_polygons[i].GetColor());
		totalG = GetGValue(_polygons[i].GetColor());
		totalB = GetBValue(_polygons[i].GetColor());

		for (const PointLight& light : lights)
		{
			Vector3D lightVector = light.GetSourcePosition().GenerateVector(_transformedVertices[i]);
			float dotProduct = Vector3D::DotProduct(lightVector.GetUnitVector(), _transformedVertices[i].GetNormalVector().GetUnitVector());

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

		_transformedVertices[i].SetColor(totalR, totalG, totalB);
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
		const Polygon3D polygon = _polygons[i];

		Vertex vertex0 = _transformedVertices[polygon.GetIndex(0)];
		Vertex vertex1 = _transformedVertices[polygon.GetIndex(1)];
		Vertex vertex2 = _transformedVertices[polygon.GetIndex(2)];

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
