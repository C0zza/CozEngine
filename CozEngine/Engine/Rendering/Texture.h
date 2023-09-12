#pragma once

enum class ETextureType
{
	Diffuse,
	Specular
};

class LTexture
{
public:
	LTexture(const unsigned int ID, const ETextureType Type, const char* Path);
	LTexture(const char* TexturePath, const bool HasAlpha, const ETextureType i_TextureType);

	void Use(const unsigned int TextureUnit) const;

	ETextureType GetTextureType() const { return TextureType; }

	static void SetFlipVerticallyOnLoad(const bool Flip);

private:
	void Init(const char* TexturePath, const bool HasAlpha);

	unsigned int TextureID;

	int Width;
	int Height;

	ETextureType TextureType;

	static const char* TextureDirectory;
};

