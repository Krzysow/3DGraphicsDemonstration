#include "PointLight.h"

PointLight::PointLight(int red, int green, int blue, Vertex position, float a, float b, float c)
{
	if (red >= 0 && red <= 255)
	{
		_redIntensity = red;
	}
	else
	{
		_redIntensity = 0;
	}
	if (green >= 0 && green <= 255)
	{
		_greenIntensity = green;
	}
	else
	{
		_greenIntensity = 0;
	}
	if (blue >= 0 && blue <= 255)
	{
		_blueIntensity = blue;
	}
	else
	{
		_blueIntensity = 0;
	}
	_sourcePosition = position;
	_a = a;
	_b = b;
	_c = c;
}

PointLight::~PointLight()
{
}

int PointLight::GetRedIntensity() const
{
	return _redIntensity;
}

int PointLight::GetGreenIntensity() const
{
	return _greenIntensity;
}

int PointLight::GetBlueIntensity() const
{
	return _blueIntensity;
}

Vertex PointLight::GetSourcePosition() const
{
	return _sourcePosition;
}

float PointLight::GetA() const
{
	return _a;
}

float PointLight::GetB() const
{
	return _b;
}

float PointLight::GetC() const
{
	return _c;
}
