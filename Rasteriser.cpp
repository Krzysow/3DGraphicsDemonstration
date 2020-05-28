#include "Rasteriser.h"
#include <cmath>
#include <algorithm>

Rasteriser app;

bool Rasteriser::Initialise()
{
	if (!MD2Loader::LoadModel("models\\marvin.md2", "textures\\marvin.pcx", _model,
							  &Model::AddPolygon,
							  &Model::AddVertex,
							  &Model::AddTextureUV))
	{
		return false;
	}

	_directionalLights.push_back(DirectionalLight(200, 255, 150, Vector3D(5, -10, 1)));
	_pointLights.push_back(PointLight( 100, 100, 100, Vertex(50, 0, -50), 0, 1.0f, 0 ));
	_spotLights.push_back(SpotLight(200, 200, 200, Vertex(0, 0, -50), 0, 1.0f, 0, Vector3D(0, 0, 1), 2.0f, 15.0f));

	return true;
}

void Rasteriser::Update(const Bitmap& bitmap)
{
	_x += 2.0f;
	_y += 3.0f;
	_z += 4.0f;

	if (_state == 0)
	{
		_modelMatrix = RotateXYZ(Matrix::toRadians(_x), Matrix::toRadians(_y), Matrix::toRadians(_z));
	}
	else if (_state == 1)
	{
		_modelMatrix = Scale(sin(Matrix::toRadians(_x)) + 1.5f, sin(Matrix::toRadians(_y)) + 1.5f, sin(Matrix::toRadians(_z)) + 1.5f);
	}
	else if (_state == 2)
	{
		_modelMatrix = Translate(20 * sin(Matrix::toRadians(_x)), 5 * sin(Matrix::toRadians(_y)), sin(Matrix::toRadians(_z)));
	}
	else
	{
		_modelMatrix = RotateXYZ(0, Matrix::toRadians(_y), 0);
	}

	GeneratePerspectiveMatrix(1, static_cast<float>(bitmap.GetWidth() / bitmap.GetHeight()));
	GenerateViewMatrix(1, bitmap.GetWidth(), bitmap.GetHeight());
}

void Rasteriser::Render(const Bitmap& bitmap)
{
	_frameCount++;
	bitmap.Clear(RGB(0, 0, 0));

	if (_state == 0)
	{
		_model.ApplyTransformToLocalVertices(_modelMatrix);
		_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
		_model.DehomogenizeAll();
		_model.ApplyTransformToTransformedVertices(_viewMatrix);
		DrawWireFrame(bitmap);
		DrawString(bitmap, L"Wireframe With Rotation");
	}
	else if (_state == 1)
	{
		_model.ApplyTransformToLocalVertices(_modelMatrix);
		_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
		_model.DehomogenizeAll();
		_model.ApplyTransformToTransformedVertices(_viewMatrix);
		DrawWireFrame(bitmap);
		DrawString(bitmap, L"Wireframe With Scaling");
	}
	else if (_state == 2)
	{
		_model.ApplyTransformToLocalVertices(_modelMatrix);
		_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
		_model.DehomogenizeAll();
		_model.ApplyTransformToTransformedVertices(_viewMatrix);
		DrawWireFrame(bitmap);
		DrawString(bitmap, L"Wireframe With Translation");
	}

	else if (_state == 3)
	{
		_model.ApplyTransformToLocalVertices(_modelMatrix);
		_model.CalculateBackfaces(_camera.GetPosition());
		_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
		_model.DehomogenizeAll();
		_model.ApplyTransformToTransformedVertices(_viewMatrix);
		DrawWireFrame(bitmap);
		DrawString(bitmap, L"Back Face Culling");
	}
	else if (_state == 4)
	{
		_model.ApplyTransformToLocalVertices(_modelMatrix);
		_model.CalculateBackfaces(_camera.GetPosition());
		_model.CalculateLightingAmbient(_ambientLight);
		_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
		_model.Sort();
		_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
		_model.DehomogenizeAll();
		_model.ApplyTransformToTransformedVertices(_viewMatrix);
		DrawSolidFlat(bitmap);
		DrawString(bitmap, L"Ambient Light");

	}
	else if (_state == 5)
	{
		_model.ApplyTransformToLocalVertices(_modelMatrix);
		_model.CalculateBackfaces(_camera.GetPosition());
		_model.CalculateLightingAmbient(_ambientLight);
		_model.CalculateLightingDirectional(_directionalLights);
		_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
		_model.Sort();
		_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
		_model.DehomogenizeAll();
		_model.ApplyTransformToTransformedVertices(_viewMatrix);
		DrawSolidFlat(bitmap);
		DrawString(bitmap, L"Directional Light");

	}
	else if (_state == 6)
	{
		_model.ApplyTransformToLocalVertices(_modelMatrix);
		_model.CalculateBackfaces(_camera.GetPosition());
		_model.CalculateLightingAmbient(_ambientLight);
		_model.CalculateLightingDirectional(_directionalLights);
		_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
		_model.Sort();
		_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
		_model.DehomogenizeAll();
		_model.ApplyTransformToTransformedVertices(_viewMatrix);
		MyDrawSolidFlat(bitmap);
		DrawString(bitmap, L"Flat Shading Using SetPixel");

	}
	else if (_state == 7)
	{
		_model.ApplyTransformToLocalVertices(_modelMatrix);
		_model.CalculateBackfaces(_camera.GetPosition());
		_model.CalculateLightingAmbient(_ambientLight);
		_model.CalculateLightingDirectional(_directionalLights);
		_model.CalculateLightingPoint(_pointLights);
		_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
		_model.Sort();
		_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
		_model.DehomogenizeAll();
		_model.ApplyTransformToTransformedVertices(_viewMatrix);
		MyDrawSolidFlat(bitmap);
		DrawString(bitmap, L"Point Light");
	}
	else if (_state == 8)
	{
		_model.ApplyTransformToLocalVertices(_modelMatrix);
		_model.CalculateBackfaces(_camera.GetPosition());
		_model.CalculateNormalVectorForVertices();
		_model.CalculateLightingAmbientSmooth(_ambientLight);
		_model.CalculateLightingDirectionalSmooth(_directionalLights);
		_model.CalculateLightingPointSmooth(_pointLights);
		_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
		_model.Sort();
		_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
		_model.DehomogenizeAll();
		_model.ApplyTransformToTransformedVertices(_viewMatrix);
		DrawGouraud(bitmap);
		DrawString(bitmap, L"Gouraud Shading");
	}
	else if (_state == 9)
	{
		_model.ApplyTransformToLocalVertices(_modelMatrix);
		_model.CalculateBackfaces(_camera.GetPosition());
		_model.CalculateNormalVectorForVertices();
		_model.CalculateLightingAmbientSmooth(_ambientLight);
		_model.CalculateLightingDirectionalSmoothWithSpecular(_directionalLights, _camera.GetPosition());
		_model.CalculateLightingPointSmoothWithSpecular(_pointLights, _camera.GetPosition());
		_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
		_model.Sort();
		_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
		_model.DehomogenizeAll();
		_model.ApplyTransformToTransformedVertices(_viewMatrix);
		DrawGouraud(bitmap);
		DrawString(bitmap, L"Specular Lighting");
	}
	else if (_state == 10)
	{
		_model.ApplyTransformToLocalVertices(_modelMatrix);
		_model.CalculateBackfaces(_camera.GetPosition());
		_model.CalculateNormalVectorForVertices();
		_model.CalculateLightingAmbientSmooth(_ambientLight);
		_model.CalculateLightingDirectionalSmoothWithSpecular(_directionalLights, _camera.GetPosition());
		_model.CalculateLightingPointSmoothWithSpecular(_pointLights, _camera.GetPosition());
		_model.CalculateLightingSpotSmoothWithSpecular(_spotLights, _camera.GetPosition());
		_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
		_model.Sort();
		_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
		_model.DehomogenizeAll();
		_model.ApplyTransformToTransformedVertices(_viewMatrix);
		DrawGouraud(bitmap);
		DrawString(bitmap, L"Spot Light");
	}
	else if (_state == 11)
	{
		_model.ApplyTransformToLocalVertices(_modelMatrix);
		_model.CalculateBackfaces(_camera.GetPosition());
		_model.CalculateNormalVectorForVertices();
		_model.CalculateLightingAmbientSmooth(_ambientLight);
		_model.CalculateLightingDirectionalSmoothWithSpecular(_directionalLights, _camera.GetPosition());
		_model.CalculateLightingPointSmoothWithSpecular(_pointLights, _camera.GetPosition());
		_model.CalculateLightingSpotSmoothWithSpecular(_spotLights, _camera.GetPosition());
		_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
		_model.Sort();
		_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
		_model.DehomogenizeAll();
		_model.ApplyTransformToTransformedVertices(_viewMatrix);
		DrawSolidTextured(bitmap);
		DrawString(bitmap, L"Txturing With Perspective Correction");
	}
	else if (_state == 12)
	{
		if (_isMarvin)
		{
			_model.Clear();
			MD2Loader::LoadModel("models\\cube.md2", "textures\\lines.pcx", _model,
				&Model::AddPolygon,
				&Model::AddVertex,
				&Model::AddTextureUV);
			_isMarvin = false;
		}

		_model.ApplyTransformToLocalVertices(_modelMatrix);
		_model.CalculateBackfaces(_camera.GetPosition());
		_model.CalculateNormalVectorForVertices();
		_model.CalculateLightingAmbientSmooth(_ambientLight);
		_model.CalculateLightingDirectionalSmoothWithSpecular(_directionalLights, _camera.GetPosition());
		_model.CalculateLightingPointSmoothWithSpecular(_pointLights, _camera.GetPosition());
		_model.CalculateLightingSpotSmoothWithSpecular(_spotLights, _camera.GetPosition());
		_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
		_model.Sort();
		_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
		_model.DehomogenizeAll();
		_model.ApplyTransformToTransformedVertices(_viewMatrix);
		DrawSolidTextured(bitmap);
		DrawString(bitmap, L"Texturing With Perspective Correction");
	}
	else if (_state == 13)
	{
		if (!_isMarvin)
		{
			_model.Clear();
			MD2Loader::LoadModel("models\\marvin.md2", "textures\\marvin.pcx", _model,
				&Model::AddPolygon,
				&Model::AddVertex,
				&Model::AddTextureUV);
			_isMarvin = true;
		}

		_model.ApplyTransformToLocalVertices(_modelMatrix);
		_model.CalculateBackfaces(_camera.GetPosition());
		_model.CalculateNormalVectorForVertices();
		_model.ApplyTransformToTransformedVertices(_camera.RetrieveCameraMatrix());
		_model.Sort();
		_model.ApplyTransformToTransformedVertices(_perspectiveMatrix);
		_model.DehomogenizeAll();
		_model.ApplyTransformToTransformedVertices(_viewMatrix);
		DrawPhong(bitmap);
		DrawString(bitmap, L"Phong Shading");
	}
	else
	{
		_model.Clear();
		MD2Loader::LoadModel("models\\marvin.md2", "textures\\marvin.pcx", _model,
			&Model::AddPolygon,
			&Model::AddVertex,
			&Model::AddTextureUV);
		_isMarvin = true;
		_state = 0;
	}

	if (_frameCount >= 150)
	{
		_frameCount = 0;
		_state++;
	}
}

void Rasteriser::DrawString(const Bitmap& bitmap, LPCTSTR text)
{
	HDC hdc = bitmap.GetDC();
	HFONT hFont, hOldFont;

	// Retrieve a handle to the variable stock font.  
	hFont = hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Myfont"));

	// Select the variable stock font into the specified device context. 
	if (hOldFont = (HFONT)SelectObject(hdc, hFont))
	{
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkColor(hdc, RGB(0, 0, 0));

		// Display the text string.  
		TextOut(hdc, 10, 10, text, lstrlen(text));

		// Restore the original font.        
		SelectObject(hdc, hOldFont);
	}
	DeleteObject(hFont);
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
	_viewMatrix = { width / 2.0f, 0, 0, width / 2.0f,
					0, -height / 2.0f, 0, height / 2.0f,
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

void Rasteriser::MyDrawSolidFlat(const Bitmap& bitmap)
{
	const HDC& hDc = bitmap.GetDC();

	const vector<Polygon3D>& polygons = _model.GetPolygons();
	const vector<Vertex>& vertices = _model.GetTransformedVertices();

	for (size_t i = 0; i < _model.GetPolygonCount(); i++)
	{
		const Polygon3D& polygon = polygons[i];

		if (polygon.GetVisibility())
		{
			//sorting by Y
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

			if (vertex0.GetY() > vertex2.GetY())
			{
				temp = vertex0;
				vertex0 = vertex2;
				vertex2 = temp;
			}

			if (vertex1.GetY() > vertex2.GetY())
			{
				temp = vertex1;
				vertex1 = vertex2;
				vertex2 = temp;
			}

			//trivial cases
			if (vertex1.GetY() == vertex2.GetY())
			{
				//sorting by x
				if (vertex2.GetX() < vertex1.GetX())
				{
					FillTopPolygonFlat(hDc, vertex0, vertex2, vertex1, polygon.GetColor());
				}
				else
				{
					FillTopPolygonFlat(hDc, vertex0, vertex1, vertex2, polygon.GetColor());
				}
			}

			else if (vertex0.GetY() == vertex1.GetY())
			{
				//sorting by x
				if (vertex1.GetX() < vertex0.GetX())
				{
					FillBottomPolygonFlat(hDc, vertex1, vertex0, vertex2, polygon.GetColor());
				}
				else
				{
					FillBottomPolygonFlat(hDc, vertex0, vertex1, vertex2, polygon.GetColor());
				}
			}

			//usual case
			else
			{
				//location of the 4th vertex
				Vertex temp(
					(vertex0.GetX() + ((vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY())) * (vertex2.GetX() - vertex0.GetX())),
						vertex1.GetY(),
						vertex1.GetZ());

				//major left
				if (vertex1.GetX() < temp.GetX())
				{
					FillTopPolygonFlat(hDc, vertex0, vertex1, temp, polygon.GetColor());
					FillBottomPolygonFlat(hDc, vertex1, temp, vertex2, polygon.GetColor());
				}
				//major right
				else
				{
					FillTopPolygonFlat(hDc, vertex0, temp, vertex1, polygon.GetColor());
					FillBottomPolygonFlat(hDc, temp, vertex1, vertex2, polygon.GetColor());
				}
			}
		}
	}
}

void Rasteriser::FillTopPolygonFlat(const HDC& hDc, const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2, const COLORREF& color)
{
	float slope0 = (vertex1.GetX() - vertex0.GetX()) / (vertex1.GetY() - vertex0.GetY());
	float slope1 = (vertex2.GetX() - vertex0.GetX()) / (vertex2.GetY() - vertex0.GetY());

	const int yStart = static_cast<int>(ceil(vertex0.GetY() - 0.5f));
	const int yEnd = static_cast<int>(ceil(vertex2.GetY() - 0.5f));

	for (int y = yStart; y < yEnd; y++)
	{
		const float px0 = slope0 * (static_cast<float>(y) + 0.5f - vertex0.GetY()) + vertex0.GetX();
		const float px1 = slope1 * (static_cast<float>(y) + 0.5f - vertex0.GetY()) + vertex0.GetX();

		const int xStart = static_cast<int>(ceil(px0 - 0.5f));
		const int xEnd = static_cast<int>(ceil(px1 - 0.5f));

		for (int x = xStart; x < xEnd; x++)
		{
			SetPixel(hDc, x, y, color);
		}
	}
}

void Rasteriser::FillBottomPolygonFlat(const HDC& hDc, const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2, const COLORREF& color)
{
	float slope0 = (vertex2.GetX() - vertex0.GetX()) / (vertex2.GetY() - vertex0.GetY());
	float slope1 = (vertex2.GetX() - vertex1.GetX()) / (vertex2.GetY() - vertex1.GetY());

	const int yStart = static_cast<int>(ceil(vertex0.GetY() - 0.5f));
	const int yEnd = static_cast<int>(ceil(vertex2.GetY() - 0.5f));

	for (int y = yStart; y < yEnd; y++)
	{
		const float px0 = slope0 * (static_cast<float>(y) + 0.5f - vertex0.GetY()) + vertex0.GetX();
		const float px1 = slope1 * (static_cast<float>(y) + 0.5f - vertex1.GetY()) + vertex1.GetX();

		const int xStart = static_cast<int>(ceil(px0 - 0.5f));
		const int xEnd = static_cast<int>(ceil(px1 - 0.5f));

		for (int x = xStart; x < xEnd; x++)
		{
			SetPixel(hDc, x, y, color);
		}
	}
}

void Rasteriser::DrawGouraud(const Bitmap& bitmap)
{
	const HDC& hDc = bitmap.GetDC();

	const vector<Polygon3D>& polygons = _model.GetPolygons();
	const vector<Vertex>& vertices = _model.GetTransformedVertices();

	for (size_t i = 0; i < _model.GetPolygonCount(); i++)
	{
		const Polygon3D& polygon = polygons[i];

		if (polygon.GetVisibility())
		{
			//sorting by Y
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

			if (vertex0.GetY() > vertex2.GetY())
			{
				temp = vertex0;
				vertex0 = vertex2;
				vertex2 = temp;
			}

			if (vertex1.GetY() > vertex2.GetY())
			{
				temp = vertex1;
				vertex1 = vertex2;
				vertex2 = temp;
			}

			//trivial cases
			if (vertex1.GetY() == vertex2.GetY())
			{
				//sorting by x
				if (vertex2.GetX() < vertex1.GetX())
				{
					FillTopPolygonGouraud(hDc, vertex0, vertex2, vertex1);
				}
				else
				{
					FillTopPolygonGouraud(hDc, vertex0, vertex1, vertex2);
				}
			}

			else if (vertex0.GetY() == vertex1.GetY())
			{
				//sorting by x
				if (vertex1.GetX() < vertex0.GetX())
				{
					FillBottomPolygonGouraud(hDc, vertex1, vertex0, vertex2);
				}
				else
				{
					FillBottomPolygonGouraud(hDc, vertex0, vertex1, vertex2);
				}
			}

			//usual case
			else
			{
				//location and color of the 4th vertex
				Vertex temp(
					vertex0.GetX() + (vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY()) * (vertex2.GetX() - vertex0.GetX()),
					vertex1.GetY(),
					vertex1.GetZ());

				float red = GetRValue(vertex0.GetColor()) + ((vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY())) * (GetRValue(vertex2.GetColor()) - GetRValue(vertex0.GetColor()));
				float green = GetGValue(vertex0.GetColor()) + ((vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY())) * (GetGValue(vertex2.GetColor()) - GetGValue(vertex0.GetColor()));
				float blue = GetBValue(vertex0.GetColor()) + ((vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY())) * (GetBValue(vertex2.GetColor()) - GetBValue(vertex0.GetColor()));
				temp.SetColor(static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue));

				//major left
				if (vertex1.GetX() < temp.GetX())
				{
					FillTopPolygonGouraud(hDc, vertex0, vertex1, temp);
					FillBottomPolygonGouraud(hDc, vertex1, temp, vertex2);
				}
				//major right
				else
				{
					FillTopPolygonGouraud(hDc, vertex0, temp, vertex1);
					FillBottomPolygonGouraud(hDc, temp, vertex1, vertex2);
				}
			}
		}
	}
}

void Rasteriser::FillTopPolygonGouraud(const HDC& hDc, const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2)
{
	const COLORREF& color0 = vertex0.GetColor();
	const COLORREF& color1 = vertex1.GetColor();
	const COLORREF& color2 = vertex2.GetColor();

	float v1v0Diff = vertex1.GetY() - vertex0.GetY();
	float v2v0Diff = vertex2.GetY() - vertex0.GetY();

	float slope0 = (vertex1.GetX() - vertex0.GetX()) / v1v0Diff;
	float slope1 = (vertex2.GetX() - vertex0.GetX()) / v2v0Diff;

	//color slopes for both sides of the polygon
	float colorSlopeRed0 = (GetRValue(color1) - GetRValue(color0)) / v1v0Diff;
	float colorSlopeGreen0 = (GetGValue(color1) - GetGValue(color0)) / v1v0Diff;
	float colorSlopeBlue0 = (GetBValue(color1) - GetBValue(color0)) / v1v0Diff;

	float colorSlopeRed1 = (GetRValue(color2) - GetRValue(color0)) / v2v0Diff;
	float colorSlopeGreen1 = (GetGValue(color2) - GetGValue(color0)) / v2v0Diff;
	float colorSlopeBlue1 = (GetBValue(color2) - GetBValue(color0)) / v2v0Diff;


	int yStart = static_cast<int>(ceil(vertex0.GetY() - 0.5f));
	int yEnd = static_cast<int>(ceil(vertex2.GetY() - 0.5f));

	for (int y = yStart; y < yEnd; y++)
	{
		float px0 = vertex0.GetX() + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * slope0;
		float px1 = vertex0.GetX() + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * slope1;
		
		float cRed0 = GetRValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeRed0;
		float cGreen0 = GetGValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeGreen0;
		float cBlue0 = GetBValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeBlue0;

		float cRed1 = GetRValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeRed1;
		float cGreen1 = GetGValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeGreen1;
		float cBlue1 = GetBValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeBlue1;

		float px1px0Diff = px1 - px0;

		float slopeRed = (cRed1 - cRed0) / px1px0Diff;
		float slopeGreen = (cGreen1 - cGreen0) / px1px0Diff;
		float slopeBlue = (cBlue1 - cBlue0) / px1px0Diff;

		const int xStart = static_cast<int>(ceil(px0 - 0.5f));
		const int xEnd = static_cast<int>(ceil(px1 - 0.5f));

		for (int x = xStart; x < xEnd; x++)
		{
			float red = cRed0 + (static_cast<float>(x) + 0.5f - px0) * slopeRed;
			float green = cGreen0 + (static_cast<float>(x) + 0.5f - px0) * slopeGreen;
			float blue = cBlue0 + (static_cast<float>(x) + 0.5f - px0) * slopeBlue;

			COLORREF color = RGB(red, green, blue);
			SetPixel(hDc, x, y, color);
		}
	}
}

void Rasteriser::FillBottomPolygonGouraud(const HDC& hDc, const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2)
{
	const COLORREF& color0 = vertex0.GetColor();
	const COLORREF& color1 = vertex1.GetColor();
	const COLORREF& color2 = vertex2.GetColor();

	float v2v0Diff = vertex2.GetY() - vertex0.GetY();
	float v2v1Diff = vertex2.GetY() - vertex1.GetY();

	float slope0 = (vertex2.GetX() - vertex0.GetX()) / v2v0Diff;
	float slope1 = (vertex2.GetX() - vertex1.GetX()) / v2v1Diff;

	//color slopes for both sides of the polygon
	float colorSlopeRed0 = (GetRValue(color2) - GetRValue(color0)) / v2v0Diff;
	float colorSlopeGreen0 = (GetGValue(color2) - GetGValue(color0)) / v2v0Diff;
	float colorSlopeBlue0 = (GetBValue(color2) - GetBValue(color0)) / v2v0Diff;

	float colorSlopeRed1 = (GetRValue(color2) - GetRValue(color1)) / v2v1Diff;
	float colorSlopeGreen1 = (GetGValue(color2) - GetGValue(color1)) / v2v1Diff;
	float colorSlopeBlue1 = (GetBValue(color2) - GetBValue(color1)) / v2v1Diff;

	int yStart = static_cast<int>(ceil(vertex0.GetY() - 0.5f));
	int yEnd = static_cast<int>(ceil(vertex2.GetY() - 0.5f));

	for (int y = yStart; y < yEnd; y++)
	{
		float px0 = vertex0.GetX() + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * slope0;
		float px1 = vertex1.GetX() + (static_cast<float>(y) + 0.5f - vertex1.GetY()) * slope1;

		float cRed0 = GetRValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeRed0;
		float cGreen0 = GetGValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeGreen0;
		float cBlue0 = GetBValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeBlue0;

		float cRed1 = GetRValue(color1) + (static_cast<float>(y) + 0.5f - vertex1.GetY()) * colorSlopeRed1;
		float cGreen1 = GetGValue(color1) + (static_cast<float>(y) + 0.5f - vertex1.GetY()) * colorSlopeGreen1;
		float cBlue1 = GetBValue(color1) + (static_cast<float>(y) + 0.5f - vertex1.GetY()) * colorSlopeBlue1;
		
		float px1px0Diff = px1 - px0;

		float slopeRed = (cRed1 - cRed0) / px1px0Diff;
		float slopeGreen = (cGreen1 - cGreen0) / px1px0Diff;
		float slopeBlue = (cBlue1 - cBlue0) / px1px0Diff;

		int xStart = static_cast<int>(ceil(px0 - 0.5f));
		int xEnd = static_cast<int>(ceil(px1 - 0.5f));

		for (int x = xStart; x < xEnd; x++)
		{
			float red = cRed0 + (static_cast<float>(x) + 0.5f - px0) * slopeRed;
			float green = cGreen0 + (static_cast<float>(x) + 0.5f - px0) * slopeGreen;
			float blue = cBlue0 + (static_cast<float>(x) + 0.5f - px0) * slopeBlue;

			COLORREF color = RGB(red, green, blue);
			SetPixel(hDc, x, y, color);
		}
	}
}

void Rasteriser::DrawSolidTextured(const Bitmap& bitmap)
{
	const HDC& hDc = bitmap.GetDC();

	const vector<Polygon3D>& polygons = _model.GetPolygons();
	const vector<Vertex>& vertices = _model.GetTransformedVertices();

	for (size_t i = 0; i < _model.GetPolygonCount(); i++)
	{
		const Polygon3D& polygon = polygons[i];

		if (polygon.GetVisibility())
		{
			//sorting by Y
			Vertex vertex0 = vertices[polygon.GetIndex(0)];
			Vertex vertex1 = vertices[polygon.GetIndex(1)];
			Vertex vertex2 = vertices[polygon.GetIndex(2)];

			vertex0.SetUOverZ(_model.GetUV()[polygon.GetUVIndex(0)].GetU() / vertex0.GetPerspectiveZ());
			vertex0.SetVOverZ(_model.GetUV()[polygon.GetUVIndex(0)].GetV() / vertex0.GetPerspectiveZ());
			vertex0.SetZRecip(1 / vertex0.GetPerspectiveZ());
			vertex1.SetUOverZ(_model.GetUV()[polygon.GetUVIndex(1)].GetU() / vertex1.GetPerspectiveZ());
			vertex1.SetVOverZ(_model.GetUV()[polygon.GetUVIndex(1)].GetV() / vertex1.GetPerspectiveZ());
			vertex1.SetZRecip(1 / vertex1.GetPerspectiveZ());
			vertex2.SetUOverZ(_model.GetUV()[polygon.GetUVIndex(2)].GetU() / vertex2.GetPerspectiveZ());
			vertex2.SetVOverZ(_model.GetUV()[polygon.GetUVIndex(2)].GetV() / vertex2.GetPerspectiveZ());
			vertex2.SetZRecip(1 / vertex2.GetPerspectiveZ());

			Vertex temp;

			if (vertex0.GetY() > vertex1.GetY())
			{
				temp = vertex0;
				vertex0 = vertex1;
				vertex1 = temp;
			}

			if (vertex0.GetY() > vertex2.GetY())
			{
				temp = vertex0;
				vertex0 = vertex2;
				vertex2 = temp;
			}

			if (vertex1.GetY() > vertex2.GetY())
			{
				temp = vertex1;
				vertex1 = vertex2;
				vertex2 = temp;
			}
			
			//trivial cases
			if (vertex1.GetY() == vertex2.GetY())
			{
				//sorting by x
				if (vertex2.GetX() < vertex1.GetX())
				{
					FillTopSolidTextured(hDc, vertex0, vertex2, vertex1);
				}
				else
				{
					FillTopSolidTextured(hDc, vertex0, vertex1, vertex2);
				}
			}

			else if (vertex0.GetY() == vertex1.GetY())
			{
				//sorting by x
				if (vertex1.GetX() < vertex0.GetX())
				{
					FillBottomSolidTextured(hDc, vertex1, vertex0, vertex2);
				}
				else
				{
					FillBottomSolidTextured(hDc, vertex0, vertex1, vertex2);
				}
			}

			//usual case
			else
			{
				//location, color and texture of the 4th vertex
				temp = Vertex(
					vertex0.GetX() + (vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY()) * (vertex2.GetX() - vertex0.GetX()),
					vertex1.GetY(),
					vertex1.GetZ());

				float red = GetRValue(vertex0.GetColor()) + ((vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY())) * (GetRValue(vertex2.GetColor()) - GetRValue(vertex0.GetColor()));
				float green = GetGValue(vertex0.GetColor()) + ((vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY())) * (GetGValue(vertex2.GetColor()) - GetGValue(vertex0.GetColor()));
				float blue = GetBValue(vertex0.GetColor()) + ((vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY())) * (GetBValue(vertex2.GetColor()) - GetBValue(vertex0.GetColor()));
				temp.SetColor(static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue));

				temp.SetUOverZ(vertex0.GetUOverZ() + (vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY()) * (vertex2.GetUOverZ() - vertex0.GetUOverZ()));
				temp.SetVOverZ(vertex0.GetVOverZ() + (vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY()) * (vertex2.GetVOverZ() - vertex0.GetVOverZ()));
				temp.SetZRecip(vertex0.GetZRecip() + (vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY()) * (vertex2.GetZRecip() - vertex0.GetZRecip()));

				//major left
				if (vertex1.GetX() < temp.GetX())
				{
					FillTopSolidTextured(hDc, vertex0, vertex1, temp);
					FillBottomSolidTextured(hDc, vertex1, temp, vertex2);
				}
				//major right
				else
				{
					FillTopSolidTextured(hDc, vertex0, temp, vertex1);
					FillBottomSolidTextured(hDc, temp, vertex1, vertex2);
				}
			}
		}
	}
}

void Rasteriser::FillTopSolidTextured(const HDC& hDc, const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2)
{
	const COLORREF& color0 = vertex0.GetColor();
	const COLORREF& color1 = vertex1.GetColor();
	const COLORREF& color2 = vertex2.GetColor();

	const float& UOverZ0 = vertex0.GetUOverZ();
	const float& VOverZ0 = vertex0.GetVOverZ();
	const float& ZRecip0 = vertex0.GetZRecip();

	const float& UOverZ1 = vertex1.GetUOverZ();
	const float& VOverZ1 = vertex1.GetVOverZ();
	const float& ZRecip1 = vertex1.GetZRecip();

	const float& UOverZ2 = vertex2.GetUOverZ();
	const float& VOverZ2 = vertex2.GetVOverZ();
	const float& ZRecip2 = vertex2.GetZRecip();

	float v1v0Diff = vertex1.GetY() - vertex0.GetY();
	float v2v0Diff = vertex2.GetY() - vertex0.GetY();

	float slope0 = (vertex1.GetX() - vertex0.GetX()) / v1v0Diff;
	float slope1 = (vertex2.GetX() - vertex0.GetX()) / v2v0Diff;

	//color slopes for both sides of the polygon
	float colorSlopeRed0 = (GetRValue(color1) - GetRValue(color0)) / v1v0Diff;
	float colorSlopeGreen0 = (GetGValue(color1) - GetGValue(color0)) / v1v0Diff;
	float colorSlopeBlue0 = (GetBValue(color1) - GetBValue(color0)) / v1v0Diff;

	float colorSlopeRed1 = (GetRValue(color2) - GetRValue(color0)) / v2v0Diff;
	float colorSlopeGreen1 = (GetGValue(color2) - GetGValue(color0)) / v2v0Diff;
	float colorSlopeBlue1 = (GetBValue(color2) - GetBValue(color0)) / v2v0Diff;

	//uv slopes for both sides of the polygon
	float UOverZSlope0 = (UOverZ1 - UOverZ0) / v1v0Diff;
	float VOverZSlope0 = (VOverZ1 - VOverZ0) / v1v0Diff;
	float ZRecipSlope0 = (ZRecip1 - ZRecip0) / v1v0Diff;

	float UOverZSlope1 = (UOverZ2 - UOverZ0) / v2v0Diff;
	float VOverZSlope1 = (VOverZ2 - VOverZ0) / v2v0Diff;
	float ZRecipSlope1 = (ZRecip2 - ZRecip0) / v2v0Diff;

	int yStart = static_cast<int>(ceil(vertex0.GetY() - 0.5f));
	int yEnd = static_cast<int>(ceil(vertex2.GetY() - 0.5f));

	for (int y = yStart; y < yEnd; y++)
	{
		float px0 = vertex0.GetX() + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * slope0;
		float px1 = vertex0.GetX() + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * slope1;

		float cRed0 = GetRValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeRed0;
		float cGreen0 = GetGValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeGreen0;
		float cBlue0 = GetBValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeBlue0;

		float cRed1 = GetRValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeRed1;
		float cGreen1 = GetGValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeGreen1;
		float cBlue1 = GetBValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY()) * colorSlopeBlue1;

		float UOverZx0 = UOverZ0 + (static_cast<float>(y) + 0.5f - vertex0.GetY())* UOverZSlope0;
		float VOverZx0 = VOverZ0 + (static_cast<float>(y) + 0.5f - vertex0.GetY())* VOverZSlope0;
		float ZRecipx0 = ZRecip0 + (static_cast<float>(y) + 0.5f - vertex0.GetY())* ZRecipSlope0;

		float UOverZx1 = UOverZ0 + (static_cast<float>(y) + 0.5f - vertex0.GetY())* UOverZSlope1;
		float VOverZx1 = VOverZ0 + (static_cast<float>(y) + 0.5f - vertex0.GetY())* VOverZSlope1;
		float ZRecipx1 = ZRecip0 + (static_cast<float>(y) + 0.5f - vertex0.GetY())* ZRecipSlope1;

		float px1px0Diff = px1 - px0;

		float slopeRed = (cRed1 - cRed0) / px1px0Diff;
		float slopeGreen = (cGreen1 - cGreen0) / px1px0Diff;
		float slopeBlue = (cBlue1 - cBlue0) / px1px0Diff;

		float UOverZSlopex = (UOverZx1 - UOverZx0) / px1px0Diff;
		float VOverZSlopex = (VOverZx1 - VOverZx0) / px1px0Diff;
		float ZRecipSlopex = (ZRecipx1 - ZRecipx0) / px1px0Diff;

		const int xStart = static_cast<int>(ceil(px0 - 0.5f));
		const int xEnd = static_cast<int>(ceil(px1 - 0.5f));

		for (int x = xStart; x < xEnd; x++)
		{
			float red = cRed0 + (static_cast<float>(x) + 0.5f - px0) * slopeRed;
			float green = cGreen0 + (static_cast<float>(x) + 0.5f - px0) * slopeGreen;
			float blue = cBlue0 + (static_cast<float>(x) + 0.5f - px0) * slopeBlue;

			float UOverZ = UOverZx0 + (static_cast<float>(x) + 0.5f - px0) * UOverZSlopex;
			float VOverZ = VOverZx0 + (static_cast<float>(x) + 0.5f - px0) * VOverZSlopex;
			float ZRecip = ZRecipx0 + (static_cast<float>(x) + 0.5f - px0) * ZRecipSlopex;

			COLORREF textureColour = _model.GetTexture().GetTextureValue(static_cast<int>(UOverZ / ZRecip), static_cast<int>(VOverZ / ZRecip));
			COLORREF color = RGB(GetRValue(textureColour) * (red / 255), GetGValue(textureColour) * (green / 255), GetBValue(textureColour) * (blue / 255));
			SetPixel(hDc, x, y, color);
		}
	}
}

void Rasteriser::FillBottomSolidTextured(const HDC& hDc, const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2)
{
	const COLORREF& color0 = vertex0.GetColor();
	const COLORREF& color1 = vertex1.GetColor();
	const COLORREF& color2 = vertex2.GetColor();

	const float& UOverZ0 = vertex0.GetUOverZ();
	const float& VOverZ0 = vertex0.GetVOverZ();
	const float& ZRecip0 = vertex0.GetZRecip();

	const float& UOverZ1 = vertex1.GetUOverZ();
	const float& VOverZ1 = vertex1.GetVOverZ();
	const float& ZRecip1 = vertex1.GetZRecip();

	const float& UOverZ2 = vertex2.GetUOverZ();
	const float& VOverZ2 = vertex2.GetVOverZ();
	const float& ZRecip2 = vertex2.GetZRecip();

	float v2v0Diff = vertex2.GetY() - vertex0.GetY();
	float v2v1Diff = vertex2.GetY() - vertex1.GetY();

	float slope0 = (vertex2.GetX() - vertex0.GetX()) / v2v0Diff;
	float slope1 = (vertex2.GetX() - vertex1.GetX()) / v2v1Diff;

	//color slopes for both sides of the polygon
	float colorSlopeRed0 = (GetRValue(color2) - GetRValue(color0)) / v2v0Diff;
	float colorSlopeGreen0 = (GetGValue(color2) - GetGValue(color0)) / v2v0Diff;
	float colorSlopeBlue0 = (GetBValue(color2) - GetBValue(color0)) / v2v0Diff;

	float colorSlopeRed1 = (GetRValue(color2) - GetRValue(color1)) / v2v1Diff;
	float colorSlopeGreen1 = (GetGValue(color2) - GetGValue(color1)) / v2v1Diff;
	float colorSlopeBlue1 = (GetBValue(color2) - GetBValue(color1)) / v2v1Diff;

	//uv slopes for both sides of the polygon
	float UOverZSlope0 = (UOverZ2 - UOverZ0) / v2v0Diff;
	float VOverZSlope0 = (VOverZ2 - VOverZ0) / v2v0Diff;
	float ZRecipSlope0 = (ZRecip2 - ZRecip0) / v2v0Diff;

	float UOverZSlope1 = (UOverZ2 - UOverZ1) / v2v1Diff;
	float VOverZSlope1 = (VOverZ2 - VOverZ1) / v2v1Diff;
	float ZRecipSlope1 = (ZRecip2 - ZRecip1) / v2v1Diff;

	int yStart = static_cast<int>(ceil(vertex0.GetY() - 0.5f));
	int yEnd = static_cast<int>(ceil(vertex2.GetY() - 0.5f));

	for (int y = yStart; y < yEnd; y++)
	{
		float px0 = vertex0.GetX() + (static_cast<float>(y) + 0.5f - vertex0.GetY())* slope0;
		float px1 = vertex1.GetX() + (static_cast<float>(y) + 0.5f - vertex1.GetY())* slope1;

		float cRed0 = GetRValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY())* colorSlopeRed0;
		float cGreen0 = GetGValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY())* colorSlopeGreen0;
		float cBlue0 = GetBValue(color0) + (static_cast<float>(y) + 0.5f - vertex0.GetY())* colorSlopeBlue0;

		float cRed1 = GetRValue(color1) + (static_cast<float>(y) + 0.5f - vertex1.GetY())* colorSlopeRed1;
		float cGreen1 = GetGValue(color1) + (static_cast<float>(y) + 0.5f - vertex1.GetY())* colorSlopeGreen1;
		float cBlue1 = GetBValue(color1) + (static_cast<float>(y) + 0.5f - vertex1.GetY())* colorSlopeBlue1;

		float UOverZx0 = UOverZ0 + (static_cast<float>(y) + 0.5f - vertex0.GetY())* UOverZSlope0;
		float VOverZx0 = VOverZ0 + (static_cast<float>(y) + 0.5f - vertex0.GetY())* VOverZSlope0;
		float ZRecipx0 = ZRecip0 + (static_cast<float>(y) + 0.5f - vertex0.GetY())* ZRecipSlope0;

		float UOverZx1 = UOverZ1 + (static_cast<float>(y) + 0.5f - vertex1.GetY())* UOverZSlope1;
		float VOverZx1 = VOverZ1 + (static_cast<float>(y) + 0.5f - vertex1.GetY())* VOverZSlope1;
		float ZRecipx1 = ZRecip1 + (static_cast<float>(y) + 0.5f - vertex1.GetY())* ZRecipSlope1;

		float px1px0Diff = px1 - px0;

		float slopeRed = (cRed1 - cRed0) / px1px0Diff;
		float slopeGreen = (cGreen1 - cGreen0) / px1px0Diff;
		float slopeBlue = (cBlue1 - cBlue0) / px1px0Diff;

		float UOverZSlopex = (UOverZx1 - UOverZx0) / px1px0Diff;
		float VOverZSlopex = (VOverZx1 - VOverZx0) / px1px0Diff;
		float ZRecipSlopex = (ZRecipx1 - ZRecipx0) / px1px0Diff;

		int xStart = static_cast<int>(ceil(px0 - 0.5f));
		int xEnd = static_cast<int>(ceil(px1 - 0.5f));

		for (int x = xStart; x < xEnd; x++)
		{
			float red = cRed0 + (static_cast<float>(x) + 0.5f - px0)* slopeRed;
			float green = cGreen0 + (static_cast<float>(x) + 0.5f - px0)* slopeGreen;
			float blue = cBlue0 + (static_cast<float>(x) + 0.5f - px0)* slopeBlue;

			float UOverZ = UOverZx0 + (static_cast<float>(x) + 0.5f - px0)* UOverZSlopex;
			float VOverZ = VOverZx0 + (static_cast<float>(x) + 0.5f - px0)* VOverZSlopex;
			float ZRecip = ZRecipx0 + (static_cast<float>(x) + 0.5f - px0)* ZRecipSlopex;

			COLORREF textureColour = _model.GetTexture().GetTextureValue(static_cast<int>(UOverZ / ZRecip), static_cast<int>(VOverZ / ZRecip));
			COLORREF color = RGB(GetRValue(textureColour) * (red / 255), GetGValue(textureColour) * (green / 255), GetBValue(textureColour) * (blue / 255));
			SetPixel(hDc, x, y, color);
		}
	}
}

void Rasteriser::DrawPhong(const Bitmap& bitmap)
{
	const HDC& hDc = bitmap.GetDC();

	const vector<Polygon3D>& polygons = _model.GetPolygons();
	const vector<Vertex>& vertices = _model.GetTransformedVertices();
	const vector<Vertex>& verticesReal = _model.GetVertices();

	for (size_t i = 0; i < _model.GetPolygonCount(); i++)
	{
		const Polygon3D& polygon = polygons[i];

		if (polygon.GetVisibility())
		{
			//sorting by Y
			Vertex vertex0 = vertices[polygon.GetIndex(0)];
			Vertex vertex1 = vertices[polygon.GetIndex(1)];
			Vertex vertex2 = vertices[polygon.GetIndex(2)];

			Vertex vertexReal0 = _modelMatrix * verticesReal[polygon.GetIndex(0)] / vertex0.GetPerspectiveZ();
			Vertex vertexReal1 = _modelMatrix * verticesReal[polygon.GetIndex(1)] / vertex1.GetPerspectiveZ();
			Vertex vertexReal2 = _modelMatrix * verticesReal[polygon.GetIndex(2)] / vertex2.GetPerspectiveZ();

			Vertex temp;
			Vertex tempReal;

			if (vertex0.GetY() > vertex1.GetY())
			{
				temp = vertex0;
				vertex0 = vertex1;
				vertex1 = temp;

				tempReal = vertexReal0;
				vertexReal0 = vertexReal1;
				vertexReal1 = tempReal;
			}

			if (vertex0.GetY() > vertex2.GetY())
			{
				temp = vertex0;
				vertex0 = vertex2;
				vertex2 = temp;

				tempReal = vertexReal0;
				vertexReal0 = vertexReal2;
				vertexReal2 = tempReal;
			}

			if (vertex1.GetY() > vertex2.GetY())
			{
				temp = vertex1;
				vertex1 = vertex2;
				vertex2 = temp;

				tempReal = vertexReal1;
				vertexReal1 = vertexReal2;
				vertexReal2 = tempReal;
			}

			//trivial cases
			if (vertex1.GetY() == vertex2.GetY())
			{
				//sorting by x
				if (vertex2.GetX() < vertex1.GetX())
				{
					FillTopPhong(hDc, vertex0, vertex2, vertex1, vertexReal0, vertexReal2, vertexReal1);
				}
				else
				{
					FillTopPhong(hDc, vertex0, vertex1, vertex2, vertexReal0, vertexReal1, vertexReal2);
				}
			}

			else if (vertex0.GetY() == vertex1.GetY())
			{
				//sorting by x
				if (vertex1.GetX() < vertex0.GetX())
				{
					FillBottomPhong(hDc, vertex1, vertex0, vertex2, vertexReal1, vertexReal0, vertexReal2);
				}
				else
				{
					FillBottomPhong(hDc, vertex0, vertex1, vertex2, vertexReal0, vertexReal1, vertexReal2);
				}
			}

			//usual case
			else
			{
				//location, world space position and normal of the 4th vertex
				temp = Vertex(
					vertex0.GetX() + (vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY()) * (vertex2.GetX() - vertex0.GetX()),
					vertex1.GetY(),
					vertex1.GetZ());

				tempReal = Vertex(
					vertexReal0.GetX() + (vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY()) * (vertexReal2.GetX() - vertexReal0.GetX()),
					vertexReal0.GetY() + (vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY()) * (vertexReal2.GetY() - vertexReal0.GetY()),
					vertexReal0.GetZ() + (vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY()) * (vertexReal2.GetZ() - vertexReal0.GetZ()));

				float vectorX = vertex0.GetNormalVector().GetX() + ((vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY())) * (vertex2.GetNormalVector().GetX() - vertex0.GetNormalVector().GetX());
				float vectorY = vertex0.GetNormalVector().GetY() + ((vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY())) * (vertex2.GetNormalVector().GetY() - vertex0.GetNormalVector().GetY());
				float vectorZ = vertex0.GetNormalVector().GetZ() + ((vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY())) * (vertex2.GetNormalVector().GetZ() - vertex0.GetNormalVector().GetZ());
				temp.SetNormalVector(Vector3D(vectorX, vectorY, vectorZ).GetUnitVector());

				temp.SetPerspectiveZ(vertex0.GetPerspectiveZ() + (vertex1.GetY() - vertex0.GetY()) / (vertex2.GetY() - vertex0.GetY()) * (vertex2.GetPerspectiveZ() - vertex0.GetPerspectiveZ()));
				
				//major left
				if (vertex1.GetX() < temp.GetX())
				{
					FillTopPhong(hDc, vertex0, vertex1, temp, vertexReal0, vertexReal1, tempReal);
					FillBottomPhong(hDc, vertex1, temp, vertex2, vertexReal1, tempReal, vertexReal2);
				}
				//major right
				else
				{
					FillTopPhong(hDc, vertex0, temp, vertex1, vertexReal0, tempReal, vertexReal1);
					FillBottomPhong(hDc, temp, vertex1, vertex2, tempReal, vertexReal1, vertexReal2);
				}
			}
		}
	}
}

void Rasteriser::FillTopPhong(const HDC& hDc, const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertexReal0, const Vertex& vertexReal1, const Vertex& vertexReal2)
{
	const Vector3D& vector0 = vertex0.GetNormalVector() / vertex0.GetPerspectiveZ();
	const Vector3D& vector1 = vertex1.GetNormalVector() / vertex1.GetPerspectiveZ();
	const Vector3D& vector2 = vertex2.GetNormalVector() / vertex2.GetPerspectiveZ();

	const float& ZRecip0 = 1 / vertex0.GetPerspectiveZ();
	const float& ZRecip1 = 1 / vertex1.GetPerspectiveZ();
	const float& ZRecip2 = 1 / vertex2.GetPerspectiveZ();

	float v1v0Diff = vertex1.GetY() - vertex0.GetY();
	float v2v0Diff = vertex2.GetY() - vertex0.GetY();

	float slope0 = (vertex1.GetX() - vertex0.GetX()) / v1v0Diff;
	float slope1 = (vertex2.GetX() - vertex0.GetX()) / v2v0Diff;

	//vector slopes for both sides of the polygon
	Vector3D vectorSlope0 = (vector1 - vector0) / v1v0Diff;
	Vector3D vectorSlope1 = (vector2 - vector0) / v2v0Diff;

	//slopes for vertices in world space
	float realSlopeX0 = (vertexReal1.GetX() - vertexReal0.GetX()) / v1v0Diff;
	float realSlopeY0 = (vertexReal1.GetY() - vertexReal0.GetY()) / v1v0Diff;
	float realSlopeZ0 = (vertexReal1.GetZ() - vertexReal0.GetZ()) / v1v0Diff;

	float realSlopeX1 = (vertexReal2.GetX() - vertexReal0.GetX()) / v2v0Diff;
	float realSlopeY1 = (vertexReal2.GetY() - vertexReal0.GetY()) / v2v0Diff;
	float realSlopeZ1 = (vertexReal2.GetZ() - vertexReal0.GetZ()) / v2v0Diff;

	float ZRecipSlope0 = (ZRecip1 - ZRecip0) / v1v0Diff;
	float ZRecipSlope1 = (ZRecip2 - ZRecip0) / v2v0Diff;

	int yStart = static_cast<int>(ceil(vertex0.GetY() - 0.5f));
	int yEnd = static_cast<int>(ceil(vertex2.GetY() - 0.5f));

	for (int y = yStart; y < yEnd; y++)
	{
		float px0 = vertex0.GetX() + slope0 * (static_cast<float>(y) + 0.5f - vertex0.GetY());
		float px1 = vertex0.GetX() + slope1 * (static_cast<float>(y) + 0.5f - vertex0.GetY());

		Vector3D vectorx0 = vector0 + vectorSlope0 * (static_cast<float>(y) + 0.5f - vertex0.GetY());
		Vector3D vectorx1 = vector0 + vectorSlope1 * (static_cast<float>(y) + 0.5f - vertex0.GetY());

		float realX0 = vertexReal0.GetX() + realSlopeX0 * (static_cast<float>(y) + 0.5f - vertex0.GetY());
		float realY0 = vertexReal0.GetY() + realSlopeY0 * (static_cast<float>(y) + 0.5f - vertex0.GetY());
		float realZ0 = vertexReal0.GetZ() + realSlopeZ0 * (static_cast<float>(y) + 0.5f - vertex0.GetY());

		float realX1 = vertexReal0.GetX() + realSlopeX1 * (static_cast<float>(y) + 0.5f - vertex0.GetY());
		float realY1 = vertexReal0.GetY() + realSlopeY1 * (static_cast<float>(y) + 0.5f - vertex0.GetY());
		float realZ1 = vertexReal0.GetZ() + realSlopeZ1 * (static_cast<float>(y) + 0.5f - vertex0.GetY());

		float ZRecipx0 = ZRecip0 + ZRecipSlope0 * (static_cast<float>(y) + 0.5f - vertex0.GetY());
		float ZRecipx1 = ZRecip0 + ZRecipSlope1 * (static_cast<float>(y) + 0.5f - vertex0.GetY());

		float px1px0Diff = px1 - px0;

		Vector3D vectorSlope = (vectorx1 - vectorx0) / px1px0Diff;

		float realSlopeX = (realX1 - realX0) / px1px0Diff;
		float realSlopeY = (realY1 - realY0) / px1px0Diff;
		float realSlopeZ = (realZ1 - realZ0) / px1px0Diff;

		float ZRecipSlope = (ZRecipx1 - ZRecipx0) / px1px0Diff;

		int xStart = static_cast<int>(ceil(px0 - 0.5f));
		int xEnd = static_cast<int>(ceil(px1 - 0.5f));

		for (int x = xStart; x < xEnd; x++)
		{
			Vector3D vector = vectorx0 + vectorSlope * (static_cast<float>(x) + 0.5f - px0);

			float realX = realX0 + realSlopeX * (static_cast<float>(x) + 0.5f - px0);
			float realY = realY0 + realSlopeY * (static_cast<float>(x) + 0.5f - px0);
			float realZ = realZ0 + realSlopeZ * (static_cast<float>(x) + 0.5f - px0);

			float ZRecip = ZRecipx0 + ZRecipSlope * (static_cast<float>(x) + 0.5f - px0);

			COLORREF color = CalculateLightingPhong((vector / ZRecip).GetUnitVector(), Vertex(realX / ZRecip, realY / ZRecip, realZ / ZRecip));
			SetPixel(hDc, x, y, color);
		}
	}
}

void Rasteriser::FillBottomPhong(const HDC& hDc, const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertexReal0, const Vertex& vertexReal1, const Vertex& vertexReal2)
{
	const Vector3D& vector0 = vertex0.GetNormalVector() / vertex0.GetPerspectiveZ();
	const Vector3D& vector1 = vertex1.GetNormalVector() / vertex1.GetPerspectiveZ();
	const Vector3D& vector2 = vertex2.GetNormalVector() / vertex2.GetPerspectiveZ();

	const float& ZRecip0 = 1 / vertex0.GetPerspectiveZ();
	const float& ZRecip1 = 1 / vertex1.GetPerspectiveZ();
	const float& ZRecip2 = 1 / vertex2.GetPerspectiveZ();

	float v2v0Diff = vertex2.GetY() - vertex0.GetY();
	float v2v1Diff = vertex2.GetY() - vertex1.GetY();

	float slope0 = (vertex2.GetX() - vertex0.GetX()) / v2v0Diff;
	float slope1 = (vertex2.GetX() - vertex1.GetX()) / v2v1Diff;

	//vector slopes for both sides of the polygon
	Vector3D vectorSlope0 = (vector2 - vector0) / v2v0Diff;
	Vector3D vectorSlope1 = (vector2 - vector1) / v2v1Diff;

	//slopes for vertices in world space
	float realSlopeX0 = (vertexReal2.GetX() - vertexReal0.GetX()) / v2v0Diff;
	float realSlopeY0 = (vertexReal2.GetY() - vertexReal0.GetY()) / v2v0Diff;
	float realSlopeZ0 = (vertexReal2.GetZ() - vertexReal0.GetZ()) / v2v0Diff;

	float realSlopeX1 = (vertexReal2.GetX() - vertexReal1.GetX()) / v2v1Diff;
	float realSlopeY1 = (vertexReal2.GetY() - vertexReal1.GetY()) / v2v1Diff;
	float realSlopeZ1 = (vertexReal2.GetZ() - vertexReal1.GetZ()) / v2v1Diff;

	float ZRecipSlope0 = (ZRecip2 - ZRecip0) / v2v0Diff;
	float ZRecipSlope1 = (ZRecip2 - ZRecip1) / v2v1Diff;

	int yStart = static_cast<int>(ceil(vertex0.GetY() - 0.5f));
	int yEnd = static_cast<int>(ceil(vertex2.GetY() - 0.5f));

	for (int y = yStart; y < yEnd; y++)
	{
		float px0 = vertex0.GetX() + (static_cast<float>(y) + 0.5f - vertex0.GetY())* slope0;
		float px1 = vertex1.GetX() + (static_cast<float>(y) + 0.5f - vertex1.GetY())* slope1;

		Vector3D vectorx0 = vector0 + vectorSlope0 * (static_cast<float>(y) + 0.5f - vertex0.GetY());
		Vector3D vectorx1 = vector1 + vectorSlope1 * (static_cast<float>(y) + 0.5f - vertex1.GetY());
		
		float realX0 = vertexReal0.GetX() + realSlopeX0 * (static_cast<float>(y) + 0.5f - vertex0.GetY());
		float realY0 = vertexReal0.GetY() + realSlopeY0 * (static_cast<float>(y) + 0.5f - vertex0.GetY());
		float realZ0 = vertexReal0.GetZ() + realSlopeZ0 * (static_cast<float>(y) + 0.5f - vertex0.GetY());

		float realX1 = vertexReal1.GetX() + realSlopeX1 * (static_cast<float>(y) + 0.5f - vertex1.GetY());
		float realY1 = vertexReal1.GetY() + realSlopeY1 * (static_cast<float>(y) + 0.5f - vertex1.GetY());
		float realZ1 = vertexReal1.GetZ() + realSlopeZ1 * (static_cast<float>(y) + 0.5f - vertex1.GetY());

		float ZRecipx0 = ZRecip0 + ZRecipSlope0 * (static_cast<float>(y) + 0.5f - vertex0.GetY());
		float ZRecipx1 = ZRecip1 + ZRecipSlope1 * (static_cast<float>(y) + 0.5f - vertex1.GetY());

		float px1px0Diff = px1 - px0;

		Vector3D vectorSlope = (vectorx1 - vectorx0) / px1px0Diff;
		
		float realSlopeX = (realX1 - realX0) / px1px0Diff;
		float realSlopeY = (realY1 - realY0) / px1px0Diff;
		float realSlopeZ = (realZ1 - realZ0) / px1px0Diff;

		float ZRecipSlope = (ZRecipx1 - ZRecipx0) / px1px0Diff;

		int xStart = static_cast<int>(ceil(px0 - 0.5f));
		int xEnd = static_cast<int>(ceil(px1 - 0.5f));

		for (int x = xStart; x < xEnd; x++)
		{
			Vector3D vector = vectorx0 + vectorSlope * (static_cast<float>(x) + 0.5f - px0);

			float realX = realX0 + realSlopeX * (static_cast<float>(x) + 0.5f - px0);
			float realY = realY0 + realSlopeY * (static_cast<float>(x) + 0.5f - px0);
			float realZ = realZ0 + realSlopeZ * (static_cast<float>(x) + 0.5f - px0);

			float ZRecip = ZRecipx0 + ZRecipSlope * (static_cast<float>(x) + 0.5f - px0);

			COLORREF color = CalculateLightingPhong((vector / ZRecip).GetUnitVector(), Vertex(realX / ZRecip, realY / ZRecip, realZ / ZRecip));
			SetPixel(hDc, x, y, color);
		}
	}
}

COLORREF Rasteriser::CalculateLightingPhong(Vector3D pixelUnitNormal, Vertex pixelPosition)
{
	float red = _ambientLight.GetRedIntensity() * _model.GetAmbientRedCoefficient();
	float green = _ambientLight.GetGreenIntensity() * _model.GetAmbientGreenCoefficient();
	float blue = _ambientLight.GetBlueIntensity() * _model.GetAmbientBlueCoefficient();
	
	for (const DirectionalLight& light : _directionalLights)
	{
		Vector3D lightVector = light.GetSourceDirection();
		float dotProductDiffuse = Vector3D::DotProduct(lightVector.GetUnitVector(), pixelUnitNormal);

		if (dotProductDiffuse > 0)
		{
			float diffuseRed = light.GetRedIntensity() * _model.GetDiffuseRedCoefficient() * dotProductDiffuse;
			float diffuseGreen = light.GetGreenIntensity() * _model.GetDiffuseGreenCoefficient() * dotProductDiffuse;
			float diffuseBlue = light.GetBlueIntensity() * _model.GetDiffuseBlueCoefficient() * dotProductDiffuse;

			Vector3D reflection = pixelUnitNormal * 2 * Vector3D::DotProduct(lightVector, pixelUnitNormal) - lightVector;
			Vector3D view = _camera.GetPosition().GenerateVector(pixelPosition);
			float dotProductSpecular = max(0.0f, Vector3D::DotProduct(reflection.GetUnitVector(), view.GetUnitVector()));

			float specularRed = light.GetRedIntensity() * _model.GetSpecularRedCoefficient() * powf(dotProductSpecular, _model.GetRoughness());
			float specularGreen = light.GetGreenIntensity() * _model.GetSpecularGreenCoefficient() * powf(dotProductSpecular, _model.GetRoughness());
			float specularBlue = light.GetBlueIntensity() * _model.GetSpecularBlueCoefficient() * powf(dotProductSpecular, _model.GetRoughness());

			red += static_cast<int>(diffuseRed + specularRed);
			green += static_cast<int>(diffuseGreen + specularGreen);
			blue += static_cast<int>(diffuseBlue + specularBlue);
		}
	}
	
	for (const PointLight& light : _pointLights)
	{
		Vector3D lightVector = light.GetSourcePosition().GenerateVector(pixelPosition);
		float dotProductDiffuse = Vector3D::DotProduct(lightVector.GetUnitVector(), pixelUnitNormal);

		if (dotProductDiffuse > 0)
		{
			float d = lightVector.GetLength();
			float attenuation = 100.0f / (light.GetA() + light.GetB() * d + light.GetC() * d * d);

			float diffuseRed = light.GetRedIntensity() * _model.GetDiffuseRedCoefficient() * dotProductDiffuse * attenuation;
			float diffuseGreen = light.GetGreenIntensity() * _model.GetDiffuseGreenCoefficient() * dotProductDiffuse * attenuation;
			float diffuseBlue = light.GetBlueIntensity() * _model.GetDiffuseBlueCoefficient() * dotProductDiffuse * attenuation;

			Vector3D reflection = pixelUnitNormal * 2 * Vector3D::DotProduct(lightVector, pixelUnitNormal) - lightVector;
			Vector3D view = _camera.GetPosition().GenerateVector(pixelPosition);
			float dotProductSpecular = max(0.0f, Vector3D::DotProduct(reflection.GetUnitVector(), view.GetUnitVector()));

			float specularRed = light.GetRedIntensity() * _model.GetSpecularRedCoefficient() * powf(dotProductSpecular, _model.GetRoughness()) * attenuation;
			float specularGreen = light.GetGreenIntensity() * _model.GetSpecularGreenCoefficient() * powf(dotProductSpecular, _model.GetRoughness()) * attenuation;
			float specularBlue = light.GetBlueIntensity() * _model.GetSpecularBlueCoefficient() * powf(dotProductSpecular, _model.GetRoughness()) * attenuation;

			red += static_cast<int>(diffuseRed + specularRed);
			green += static_cast<int>(diffuseGreen + specularGreen);
			blue += static_cast<int>(diffuseBlue + specularBlue);
		}
	}
	
	for (const SpotLight& light : _spotLights)
	{
		Vector3D lightVector = light.GetSourcePosition().GenerateVector(pixelPosition);
		float dotProductDiffuse = Vector3D::DotProduct(lightVector.GetUnitVector(), pixelUnitNormal);

		if (dotProductDiffuse > 0)
		{
			float d = lightVector.GetLength();
			float attenuation = 100.0f / (light.GetA() + light.GetB() * d + light.GetC() * d * d);

			float angle = Vector3D::DotProduct(light.GetSourcePosition().GenerateVector(pixelPosition).GetUnitVector(), light.GetDirection().GetUnitVector());
			float spotLightMagic = smoothstep(light.GetOuterEdge(), light.GetInnerEdge(), angle);

			float diffuseRed = light.GetRedIntensity() * _model.GetDiffuseRedCoefficient() * dotProductDiffuse * attenuation * spotLightMagic;
			float diffuseGreen = light.GetGreenIntensity() * _model.GetDiffuseGreenCoefficient() * dotProductDiffuse * attenuation * spotLightMagic;
			float diffuseBlue = light.GetBlueIntensity() * _model.GetDiffuseBlueCoefficient() * dotProductDiffuse * attenuation * spotLightMagic;

			Vector3D reflection = pixelUnitNormal * 2 * Vector3D::DotProduct(lightVector, pixelUnitNormal) - lightVector;
			Vector3D view = _camera.GetPosition().GenerateVector(pixelPosition);
			float dotProductSpecular = max(0.0f, Vector3D::DotProduct(reflection.GetUnitVector(), view.GetUnitVector()));

			float specularRed = light.GetRedIntensity() * _model.GetSpecularRedCoefficient() * powf(dotProductSpecular, _model.GetRoughness()) * attenuation * spotLightMagic;
			float specularGreen = light.GetGreenIntensity() * _model.GetSpecularGreenCoefficient() * powf(dotProductSpecular, _model.GetRoughness()) * attenuation * spotLightMagic;
			float specularBlue = light.GetBlueIntensity() * _model.GetSpecularBlueCoefficient() * powf(dotProductSpecular, _model.GetRoughness()) * attenuation * spotLightMagic;

			red += static_cast<int>(diffuseRed + specularRed);
			green += static_cast<int>(diffuseGreen + specularGreen);
			blue += static_cast<int>(diffuseBlue + specularBlue);
		}
	}
	
	return RGB(Model::Clamp(0, 255, static_cast<int>(red)), Model::Clamp(0, 255, static_cast<int>(green)), Model::Clamp(0, 255, static_cast<int>(blue)));
}
