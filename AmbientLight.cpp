#include "AmbientLight.h"

AmbientLight::AmbientLight(int red, int green, int blue)
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
}

AmbientLight::~AmbientLight()
{
}

int AmbientLight::GetRedIntensity() const
{
	return _redIntensity;;
}

int AmbientLight::GetGreenIntensity() const
{
	return _greenIntensity;
}

int AmbientLight::GetBlueIntensity() const
{
	return _blueIntensity;
}
