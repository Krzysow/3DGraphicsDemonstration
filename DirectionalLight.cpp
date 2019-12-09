#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(int red, int green, int blue, Vector3D direction)
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
	_sourceDirection = direction;
}

DirectionalLight::~DirectionalLight()
{
}

int DirectionalLight::GetRedIntensity() const
{
	return _redIntensity;;
}

int DirectionalLight::GetGreenIntensity() const
{
	return _greenIntensity;
}

int DirectionalLight::GetBlueIntensity() const
{
	return _blueIntensity;
}

Vector3D DirectionalLight::GetSourceDirection() const
{
	return _sourceDirection;
}
