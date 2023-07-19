#pragma once

class Texture
{
public:
	Texture(const char* TexturePath);

	void Use() const;

private:
	unsigned int TextureID;

	int Width;
	int Height;

	static const char* TextureDirectory;
};

