#pragma once
#include "Vertex.h"
#include <initializer_list>

const int ROWS = 4;
const int COLS = 4;

class Matrix
{
public:
	Matrix();
	Matrix(std::initializer_list<float> list);
	Matrix(const Matrix& other);
	~Matrix();

	float GetM(const int row, const int column) const;
	void SetM(const int row, const int column, const float value);

	Matrix& operator= (const Matrix& rhs);
	bool operator== (const Matrix& other) const;
	Vertex operator* (const Vertex& other) const;
	Matrix operator* (const Matrix& other) const;

	float static toRadians(const float rotation);

private:
	float _m[ROWS][COLS]{ 0 };

	void Copy(const Matrix& other);
};

