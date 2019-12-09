#include "Matrix.h"
#include <cmath>

Matrix::Matrix() : _m{0}
{
}

Matrix::Matrix(std::initializer_list<float> list) : _m{ 0 }
{
	if (list.size() != ROWS * COLS)
	{
		throw "Insufficient values supplied";
	}

	auto iterator = list.begin();
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			_m[i][j] = *iterator++;
		}
	}
}

Matrix::Matrix(const Matrix& other)
{
	Copy(other);
}

Matrix::~Matrix()
{
}

float Matrix::GetM(const int row, const int column) const
{
	return _m[row][column];
}

void Matrix::SetM(const int row, const int column, const float value)
{
	_m[row][column] = value;
}

Matrix& Matrix::operator= (const Matrix& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

bool Matrix::operator== (const Matrix& other) const
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (_m[i][j] != other.GetM(i, j))
			{
				return false;
			}
		}
	}
	return true;
}

Vertex Matrix::operator* (const Vertex& other) const
{
	Vertex vertex;
	vertex.SetX(_m[0][0] * other.GetX() + _m[0][1] * other.GetY() + _m[0][2] * other.GetZ() + _m[0][3] * other.GetW());
	vertex.SetY(_m[1][0] * other.GetX() + _m[1][1] * other.GetY() + _m[1][2] * other.GetZ() + _m[1][3] * other.GetW());
	vertex.SetZ(_m[2][0] * other.GetX() + _m[2][1] * other.GetY() + _m[2][2] * other.GetZ() + _m[2][3] * other.GetW());
	vertex.SetW(_m[3][0] * other.GetX() + _m[3][1] * other.GetY() + _m[3][2] * other.GetZ() + _m[3][3] * other.GetW());
	return vertex;
}

Matrix Matrix::operator* (const Matrix& other) const
{
	Matrix matrix;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			float value = 0;
			for (int k = 0; k < ROWS; k++)
			{
				value += _m[i][k] * other.GetM(k, j);
			}
			matrix.SetM(i, j, value);
		}
	}
	return matrix;
}

float Matrix::toRadians(const float rotation)
{
	return static_cast<float>(rotation * 3.14 / 180.0);
}

void Matrix::Copy(const Matrix& other)
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			_m[i][j] = other.GetM(i, j);
		}
	}
}
