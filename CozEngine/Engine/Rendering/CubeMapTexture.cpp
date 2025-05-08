#include "stdafx.h"
#include "CubeMapTexture.h"

void LCubeMapTexture::SetupTexture()
{
	if (TextureFiles.size() != 6)
	{
		Log(LLogLevel::ERROR, "LCubeMapTexture::SetupTexture - Incorrect number of textures for cube map. 6 Required.");
		return;
	}

	stbi_set_flip_vertically_on_load(false);

	glBindTexture(GL_TEXTURE_CUBE_MAP, GetTextureID());

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	int Width, Height, NrChannels;
	unsigned char* data;
	// Currently dependent on the order of TextureFiles. Order should be r, l, t, b, f, b.
	for (unsigned int i = 0; i < TextureFiles.size(); ++i)
	{
		std::string FullTexturePath = TextureFiles[i];
		data = stbi_load(FullTexturePath.c_str(), &Width, &Height, &NrChannels, 0);
		if (data)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
		}
		else
		{
			Log(LLogLevel::ERROR, "LCubeMapTexture::SetupTexture - Failed to load texture" + TextureFiles[i]);
		}

		stbi_image_free(data);
	}

	stbi_set_flip_vertically_on_load(true);
}
