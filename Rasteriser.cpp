#include "Rasteriser.h"
#include <cmath>
#include <algorithm>


Rasteriser app;

bool Rasteriser::Initialise()
{
	if (!MD2Loader::LoadModel("models\\marvin.md2", _model, &Model::AddPolygon, &Model::AddVertex))
	{
		return false;
	}

	_directionalLights.push_back(DirectionalLight(255, 200, 150, Vector3D(5, -10, 1)));
	_pointLights.push_back(PointLight( 100, 100, 100, Vertex(0, 0, -50), 0, 1.0f, 0 ));

	_x = 0.0f;
	_y = 1.0f;
	_z = 0.0f;

	return true;
}

void Rasteriser::Update(const Bitmap& bitmap)
{
	_x -= 2.0f;
	_y += 1.0f;
	
	_modelMatrix = RotateXYZ(0, Matrix::toRadians(_y), 0);
	GeneratePerspectiveMatrix(1, static_cast<float>(bitmap.GetWidth() / bitmap.GetHeight())); // make  a _d variable ???
	GenerateViewMatrix(1, bitmap.GetWidth(), bitmap.GetHeight()); // _d could go here too
}

void Rasteriser::Render(const Bitmap& bitmap)
{
	bitmap.Clear(RGB(0, 0, 0));

	_model.ApplyTransformToLocalVertices(_modelMatrix);
	_model.CalculateBackfaces(_camera.GetPosition());
	_model.CalculateNormalVectorForVertices();
	_model.CalculateLightingAmbient(_ambientLight);
	_model.CalculateLightingDirectional(_directionalLights);
	_model.CalculateLightingPoint(_pointLights);
	_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
	_model.Sort();
	_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
	_model.DehomogenizeAll();
	_model.ApplyTransformToTransformedVertices(_viewMatrix);
	//DrawWireFrame(bitmap);
	//DrawSolidFlat(bitmap);
	MyDrawSolidFlat(bitmap);
}

Matrix Rasteriser::Translate(float translateX, float translateY, float translateZ)
{
	return { 1, 0, 0, translateX,
			0, 1, 0, translateY,
			0, 0, 1, translateZ,
			0, 0, 0, 1 };
}

Matrix Rasteriser::Scale(float scaleX, float scaleY, float scaleZ)
{
	return { scaleX, 0, 0, 0,
			0, scaleY, 0, 0,
			0, 0, scaleZ, 0,
			0, 0, 0, 1 };
}

Matrix Rasteriser::RotateX(float radians)
{
	return { 1, 0, 0, 0,
			0, static_cast<float>(cos(radians)), static_cast<float>(-sin(radians)), 0,
			0, static_cast<float>(sin(radians)), static_cast<float>(cos(radians)), 0,
			0, 0, 0, 1 };
}

Matrix Rasteriser::RotateY(float radians)
{
	return { static_cast<float>(cos(radians)), 0, static_cast<float>(sin(radians)), 0,
			0, 1, 0, 0,
			static_cast<float>(-sin(radians)), 0, static_cast<float>(cos(radians)), 0,
			0, 0, 0, 1 };
}

Matrix Rasteriser::RotateZ(float radians)
{
	return { static_cast<float>(cos(radians)), static_cast<float>(-sin(radians)), 0, 0,
			static_cast<float>(sin(radians)), static_cast<float>(cos(radians)), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };
}

Matrix Rasteriser::RotateXYZ(float angleX, float angleY, float angleZ)
{
	return RotateZ(angleZ) * RotateY(angleY) * RotateX(angleX);
}

void Rasteriser::GeneratePerspectiveMatrix(float d, float aspectRatio)
{
	_perspectiveMatrix = Matrix{ d / aspectRatio, 0, 0, 0,
								0, d, 0, 0,
								0, 0, d, 0,
								0, 0, 1, 0 };
}

void Rasteriser::GenerateViewMatrix(float d, int width, int height)
{
	_viewMatrix = { static_cast<float>(width / 2), 0, 0, static_cast<float>(width / 2),
					0, static_cast<float>(-height / 2), 0, static_cast<float>(height / 2),
					0, 0, d / 2, d / 2,
					0, 0, 0, 1 };
}

void Rasteriser::DrawWireFrame(const Bitmap& bitmap) 
{
	HDC hDc = bitmap.GetDC();

	const vector<Polygon3D>& polygons = _model.GetPolygons();
	const vector<Vertex>& vertices = _model.GetTransformedVertices();

	for (size_t i = 0; i < _model.GetPolygonCount(); i++)
	{
		const Polygon3D& polygon = polygons[i];

		if (polygon.GetVisibility())
		{
			MoveToEx(hDc, static_cast<int>(vertices[polygon.GetIndex(2)].GetX()), static_cast<int>(vertices[polygon.GetIndex(2)].GetY()), NULL);

			HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
			HGDIOBJ oldPen = SelectObject(hDc, hPen);

			LineTo(hDc, static_cast<int>(vertices[polygon.GetIndex(0)].GetX()), static_cast<int>(vertices[polygon.GetIndex(0)].GetY()));
			LineTo(hDc, static_cast<int>(vertices[polygon.GetIndex(1)].GetX()), static_cast<int>(vertices[polygon.GetIndex(1)].GetY()));
			LineTo(hDc, static_cast<int>(vertices[polygon.GetIndex(2)].GetX()), static_cast<int>(vertices[polygon.GetIndex(2)].GetY()));

			SelectObject(hDc, oldPen);
			DeleteObject(hPen);
		}
	}
}

void Rasteriser::DrawSolidFlat(const Bitmap& bitmap)
{
	HDC hDc = bitmap.GetDC();

	const vector<Polygon3D>& polygons = _model.GetPolygons();
	const vector<Vertex>& vertices = _model.GetTransformedVertices();

	POINT points[3];

	for (size_t i = 0; i < _model.GetPolygonCount(); i++)
	{
		const Polygon3D& polygon = polygons[i];

		if (polygon.GetVisibility())
		{
			for (int j = 0; j < 3; j++)
			{
				points[j] = { (LONG)vertices[polygon.GetIndex(j)].GetX(), (LONG)vertices[polygon.GetIndex(j)].GetY() };
			}

			HPEN hPen = CreatePen(PS_SOLID, 2, polygon.GetColor());
			HBRUSH brush = CreateSolidBrush(polygon.GetColor());
			HGDIOBJ oldPen = SelectObject(hDc, hPen);
			HGDIOBJ oldBrush = SelectObject(hDc, brush);

			Polygon(hDc, points, 3);

			SelectObject(hDc, oldPen);
			SelectObject(hDc, oldBrush);
			DeleteObject(brush);
			DeleteObject(hPen);
		}
	}
}

//void Rasteriser::MyDrawSolidFlat(const Bitmap& bitmap)
//{
//	HDC hDc = bitmap.GetDC();
//
//	const vector<Polygon3D>& polygons = _model.GetPolygons();
//	const vector<Vertex>& vertices = _model.GetTransformedVertices();
//
//	for (size_t i = 0; i < _model.GetPolygonCount(); i++)
//	{
//		const Polygon3D& polygon = polygons[i];
//
//		if (polygon.GetVisibility())
//		{
//			Vertex vertex0 = vertices[polygon.GetIndex(0)];
//			Vertex vertex1 = vertices[polygon.GetIndex(1)];
//			Vertex vertex2 = vertices[polygon.GetIndex(2)];
//
//			Vertex vTmp;
//
//			if (vertex0.GetY() > vertex1.GetY())
//			{
//				vTmp = vertex0;
//				vertex0 = vertex1;
//				vertex1 = vTmp;
//			}
//			/* here v1.GetY() <= v2.GetY() */
//			if (vertex0.GetY() > vertex2.GetY())
//			{
//				vTmp = vertex0;
//				vertex0 = vertex2;
//				vertex2 = vTmp;
//			}
//			/* here v1.GetY() <= v2.GetY() and v1.GetY() <= v3.GetY() so test v2 vs. v3 */
//			if (vertex1.GetY() > vertex2.GetY())
//			{
//				vTmp = vertex1;
//				vertex1 = vertex2;
//				vertex2 = vTmp;
//			}
//
//			FillPolygonFlat(hDc, vertex0, vertex1, vertex2, polygon.GetColor());
//			
//		}
//	}
//}

//void Rasteriser::FillPolygonFlat(const HDC& hDc, const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2, const COLORREF& color)
//{
//	HPEN hPen = CreatePen(PS_SOLID, 2, color);
//	HGDIOBJ oldPen = SelectObject(hDc, hPen);
//
//	if (vertex1.GetY() == vertex2.GetY())
//	{
//		fillBottomFlatTriangle(hDc, vertex0, vertex1, vertex2, color);
//	}
//	/* check for trivial case of top-flat triangle */
//	else if (vertex0.GetY() == vertex1.GetY())
//	{
//		fillTopFlatTriangle(hDc, vertex0, vertex1, vertex2, color);
//	}
//	else
//	{
//		/* general case - split the triangle in a topflat and bottom-flat one */
//		Vertex vTmp(
//			vertex0.GetX() + (vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY()) * (vertex2.GetX() - vertex0.GetX()),
//			vertex1.GetY(),
//			vertex1.GetZ());
//	
//		fillBottomFlatTriangle(hDc, vertex0, vertex1, vTmp, color);
//		fillTopFlatTriangle(hDc, vertex1, vTmp, vertex2, color);
//	}
//
//	SelectObject(hDc, oldPen);
//	DeleteObject(hPen);
//}
//
//void Rasteriser::fillTopFlatTriangle(const HDC& hDc, const Vertex& v1, const Vertex& v2, const Vertex& v3, const COLORREF& color)
//{
//	float slope1 = (v3.GetX() - v1.GetX()) / (v3.GetY() - v1.GetY());
//	float slope2 = (v3.GetX() - v2.GetX()) / (v3.GetY() - v2.GetY());
//
//	float x1 = v3.GetX();
//	float x2 = v3.GetX() + 0.5f;
//
//	for (float scanlineY = v3.GetY(); scanlineY > v1.GetY(); scanlineY--)
//	{
//		MoveToEx(hDc, static_cast<int>(x1), static_cast<int>(scanlineY), NULL);
//		LineTo(hDc, static_cast<int>(x2), static_cast<int>(scanlineY));
//		
//		x1 -= slope1;
//		x2 -= slope2;
//	}
//}
//
//void Rasteriser::fillBottomFlatTriangle(const HDC& hDc, const Vertex& v1, const Vertex& v2, const Vertex& v3, const COLORREF& color)
//{
//	float slope1 = (v2.GetX() - v1.GetX()) / (v2.GetY() - v1.GetY());
//	float slope2 = (v3.GetX() - v1.GetX()) / (v3.GetY() - v1.GetY());
//
//	float x1 = v1.GetX();
//	float x2 = v1.GetX() + 0.5f;
//
//	for (float scanlineY = v1.GetY(); scanlineY <= v2.GetY(); scanlineY++)
//	{
//		MoveToEx(hDc, static_cast<int>(x1), static_cast<int>(scanlineY), NULL);
//		LineTo(hDc, static_cast<int>(x2), static_cast<int>(scanlineY));
//
//		x1 += slope1;
//		x2 += slope2;
//
//	}
//}
void Rasteriser::MyDrawSolidFlat(const Bitmap& bitmap)
{
	HDC hDc = bitmap.GetDC();

	const vector<Polygon3D>& polygons = _model.GetPolygons();
	const vector<Vertex>& vertices = _model.GetTransformedVertices();

	for (size_t i = 0; i < _model.GetPolygonCount(); i++)
	{
		const Polygon3D& polygon = polygons[i];

		if (polygon.GetVisibility())
		{
			Vertex vertex0 = vertices[polygon.GetIndex(0)];
			Vertex vertex1 = vertices[polygon.GetIndex(1)];
			Vertex vertex2 = vertices[polygon.GetIndex(2)];

			Vertex temp;

			if (vertex0.GetY() > vertex1.GetY())
			{
				temp = vertex0;
				vertex0 = vertex1;
				vertex1 = temp;
			}
			/* here v1.GetY() <= v2.GetY() */
			if (vertex0.GetY() > vertex2.GetY())
			{
				temp = vertex0;
				vertex0 = vertex2;
				vertex2 = temp;
			}
			/* here v1.GetY() <= v2.GetY() and v1.GetY() <= v3.GetY() so test v2 vs. v3 */
			if (vertex1.GetY() > vertex2.GetY())
			{
				temp = vertex1;
				vertex1 = vertex2;
				vertex2 = temp;
			}

			/* here we know that v1.GetY() <= v2.GetY() <= v3.GetY() */
			/* check for trivial case of bottom-flat triangle */
			if (vertex1.GetY() == vertex2.GetY())
			{
				FillPolygonFlat(hDc, vertex0, vertex1, vertex2, polygon.GetColor());
			}
			/* check for trivial case of top-flat triangle */
			else if (vertex0.GetY() == vertex1.GetY())
			{
				FillPolygonFlat(hDc, vertex2, vertex0, vertex1, polygon.GetColor());
			}
			else
			{
				/* general case - split the triangle in a topflat and bottom-flat one */
				Vertex temp(
					vertex0.GetX() + (vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY()) * (vertex2.GetX() - vertex0.GetX()),
					vertex1.GetY(),
					vertex1.GetZ());
				
				FillPolygonFlat(hDc, vertex0, vertex1, temp, polygon.GetColor());
				FillPolygonFlat(hDc, vertex2, vertex1, temp, polygon.GetColor());
			}
		}
	}
}

int sign(float value)
{
	return (value > 0) ? 1 : (value < 0) ? -1 : 0;
}

void Rasteriser::FillPolygonFlat(const HDC& hDc, const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2, const COLORREF& color)
{
	Vertex temp1(vertex0);
	Vertex temp2(vertex0);

	bool changed1 = false;
	bool changed2 = false;

	float lengthX1 = (abs(vertex1.GetX() - vertex0.GetX()));
	float lengthY1 = (abs(vertex1.GetY() - vertex0.GetY()));

	float lengthX2 = (abs(vertex2.GetX() - vertex0.GetX()));
	float lengthY2 = (abs(vertex2.GetY() - vertex0.GetY()));

	int signX1 = sign(vertex1.GetX() - vertex0.GetX());
	int signX2 = sign(vertex2.GetX() - vertex0.GetX());

	int signY1 = sign(vertex1.GetY() - vertex0.GetY());
	int signY2 = sign(vertex2.GetY() - vertex0.GetY());

	if (lengthY1 > lengthX1)
	{   // swap values
		float temp = lengthX1;
		lengthX1 = lengthY1;
		lengthY1 = temp;
		changed1 = true;
	}

	if (lengthY2 > lengthX2)
	{   // swap values
		float temp = lengthX2;
		lengthX2 = lengthY2;
		lengthY2 = temp;
		changed2 = true;
	}

	float error1 = 2 * lengthY1 - lengthX1;
	float error2 = 2 * lengthY2 - lengthX2;


	HPEN hPen = CreatePen(PS_SOLID, 2, color);
	HGDIOBJ oldPen = SelectObject(hDc, hPen);

	for (int i = 0; i <= lengthX1; i++)
	{
		MoveToEx(hDc, static_cast<int>(temp1.GetX()), static_cast<int>(temp1.GetY()), NULL);
		LineTo(hDc, static_cast<int>(temp2.GetX()), static_cast<int>(temp2.GetY()));

		while (error1 >= 0)
		{
			if (changed1)
			{
				temp1.SetX(temp1.GetX() + signX1);
			}
			else
			{
				temp1.SetY(temp1.GetY() + signY1);
			}

			error1 = error1 - 2 * lengthX1;
		}

		if (changed1)
		{
			temp1.SetY(temp1.GetY() + signY1);
		}
		else
		{
			temp1.SetX(temp1.GetX() + signX1);
		}

		error1 = error1 + 2 * lengthY1;

		/* here we rendered the next point on line 1 so follow now line 2
		 * until we are on the same y-value as line 1.
		 */
		while (temp2.GetY() != temp1.GetY())
		{
			while (error2 >= 0)
			{
				if (changed2)
				{
					temp2.SetX(temp2.GetX() + signX2);
				}
				else
				{
					temp2.SetY(temp2.GetY() + signY2);
				}

				error2 = error2 - 2 * lengthX2;
			}

			if (changed2)
			{
				temp2.SetY(temp2.GetY() + signY2);
			}
				
			else
			{
				temp2.SetX(temp2.GetX() + signX2);
			}

			error2 = error2 + 2 * lengthY2;
		}
	}

	SelectObject(hDc, oldPen);
	DeleteObject(hPen);
}
