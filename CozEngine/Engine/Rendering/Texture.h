#pragma once

enum class ETextureType
{
	Diffuse
};

class LTexture
{
public:
	LTexture(const char* TexturePath, const bool HasAlpha, const ETextureType i_TextureType);

	void Use(const unsigned int TextureUnit = 0) const;

	ETextureType GetTextureType() const { return TextureType; }

	static void SetFlipVerticallyOnLoad(const bool Flip);

private:
	unsigned int TextureID;

	int Width;
	int Height;

	ETextureType TextureType;

	static const char* TextureDirectory;
};

