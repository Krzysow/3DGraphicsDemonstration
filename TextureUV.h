#pragma once
class TextureUV
{
public:
	TextureUV(float u, float v);
	~TextureUV();

	float GetU() const;
	float GetV() const;

	TextureUV& operator= (const TextureUV& rhs);
	
private:
	float _u;
	float _v;
};

