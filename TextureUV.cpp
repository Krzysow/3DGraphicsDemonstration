#include "TextureUV.h"

TextureUV::TextureUV(float u, float v)
{
	_u = u;
	_v = v;
}

TextureUV::~TextureUV()
{
}

float TextureUV::GetU() const
{
	return _u;
}

float TextureUV::GetV() const
{
	return _v;
}

TextureUV& TextureUV::operator=(const TextureUV& other)
{
	if (this != &other)
	{
		_u = other.GetU();
		_v = other.GetV();
	}
	return *this;
}
