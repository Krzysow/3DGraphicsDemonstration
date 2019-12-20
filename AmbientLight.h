#pragma once
#include "Light.h"

class AmbientLight : public Light
{
public:
	AmbientLight(int red, int green, int blue) : Light(red, green, blue) {}
	~AmbientLight();
};
