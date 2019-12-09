#pragma once
class AmbientLight
{
public:
	AmbientLight(int red, int green, int blue);
	~AmbientLight();

	int GetRedIntensity() const;
	int GetGreenIntensity() const;
	int GetBlueIntensity() const;

private:
	int _redIntensity;
	int _greenIntensity;
	int _blueIntensity;

};
