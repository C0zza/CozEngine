#include "Texture.h"

#include <glad/glad.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <string>

const char* Texture::TextureDirectory = "Game/Assets/";

Texture::Texture(const char* TexturePath, const bool HasAlpha)
	: Width{ 0 }, Height{ 0 }, TextureID{ 0 }
{
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int nrChannels;
	std::string FullTexturePath = TextureDirectory;
	FullTexturePath += TexturePath;

	unsigned char* data = stbi_load(FullTexturePath.c_str(), &Width, &Height, &nrChannels, 0);

	if (data)
	{
		int Format = HasAlpha ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Texture::Texture - Failed to load texture " << FullTexturePath << "\n";
	}

	stbi_image_free(data);
}

void Texture::Use(const unsigned int TextureUnit /* = 0 */) const
{
	glActiveTexture(GL_TEXTURE0 + TextureUnit);
	glBindTexture(GL_TEXTURE_2D, TextureID);
}

void Texture::SetFlipVerticallyOnLoad(const bool Flip)
{
	stbi_set_flip_vertically_on_load(Flip);
}
