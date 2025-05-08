#include "Texture.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <string>

#include "Misc/Logging.h"

void LTexture::Load()
{
	glGenTextures(1, &TextureID);

	SetupTexture();
}

void LTexture::Unload()
{
	glDeleteTextures(1, &TextureID);
}

void LTexture::Use(const unsigned int TextureUnit) const
{
	glActiveTexture(GL_TEXTURE0 + TextureUnit);
	glBindTexture(GL_TEXTURE_2D, TextureID);
}

void LTexture::SetFlipVerticallyOnLoad(const bool Flip)
{
	stbi_set_flip_vertically_on_load(Flip);
}

void LTexture::SetupTexture()
{
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (TextureFiles.empty())
	{
		return;
	}

	if (TextureFiles.size() > 1)
	{
		Log(LLogLevel::WARNING, "LTexture::SetupTexture - More than 1 texture file defined for default texture. Using first.");
	}

	int nrChannels;

	unsigned char* data = stbi_load(TextureFiles[0].c_str(), &Width, &Height, &nrChannels, 0);

	if (data)
	{
		int Format = bHasAlpha ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		Log(LLogLevel::ERROR, "LTexture::SetupTexture - Failed to load texture" + TextureFiles[0]);
	}

	stbi_image_free(data);
}
