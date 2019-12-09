#pragma once
#include "Vector3D.h"
class DirectionalLight
{
public:
	DirectionalLight(int red, int green, int blue, Vector3D direction);
	~DirectionalLight();

	int GetRedIntensity() const;
	int GetGreenIntensity() const;
	int GetBlueIntensity() const;
	Vector3D GetSourceDirection() const;

private:
	int _redIntensity;
	int _greenIntensity;
	int _blueIntensity;
	Vector3D _sourceDirection;
};

