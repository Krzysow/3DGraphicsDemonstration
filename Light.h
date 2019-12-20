#pragma once
class Light
{
public:
	Light(int red, int green, int blue);
	~Light();

	int GetRedIntensity() const;
	int GetGreenIntensity() const;
	int GetBlueIntensity() const;

protected:
	int _redIntensity;
	int _greenIntensity;
	int _blueIntensity;
};

