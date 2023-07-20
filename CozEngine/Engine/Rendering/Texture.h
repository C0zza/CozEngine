#pragma once

class Texture
{
public:
	Texture(const char* TexturePath, const bool HasAlpha);

	void Use(const unsigned int TextureUnit = 0) const;

	static void SetFlipVerticallyOnLoad(const bool Flip);

private:
	unsigned int TextureID;

	int Width;
	int Height;

	static const char* TextureDirectory;
};

