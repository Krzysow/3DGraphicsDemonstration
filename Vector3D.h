#pragma once
class Vector3D
{
public:
	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(const Vector3D& other);

	float GetX() const;
	float GetY() const;
	float GetZ() const;
	float GetLength() const;
	Vector3D GetUnitVector() const;

	void SetX(const float x);
	void SetY(const float y);
	void SetZ(const float z);

	Vector3D& operator= (const Vector3D& rhs);
	bool operator== (const Vector3D& rhs) const;
	const Vector3D operator+ (const Vector3D& rhs) const;
	const Vector3D operator- (const Vector3D& rhs) const;
	const Vector3D operator* (const float rhs) const;
	const Vector3D operator/ (const float rhs) const;
	const Vector3D operator/ (const int rhs) const;


	static float DotProduct(const Vector3D u, const Vector3D v);
	static Vector3D CrossProduct(const Vector3D u, const Vector3D v);

private:
	float _x;
	float _y;
	float _z;

	void Copy(const Vector3D& other);
};

