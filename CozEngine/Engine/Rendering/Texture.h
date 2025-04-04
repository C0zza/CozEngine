#pragma once

#include <string>
#include <vector>

#include "ResourceManagement/ResourceHandleHelper.h"

enum class ETextureType
{
	Diffuse,
	Specular
};

class LTexture : public LResource
{
public:
	virtual void Load() override;
	virtual void Unload() override;

	void Use(const unsigned int TextureUnit) const;

	unsigned int GetTextureID() const { return TextureID; }
	ETextureType GetTextureType() const { return TextureType; }
	int GetWidth() const { return Width; }
	int GetHeight() const { return Height; }

	static void SetFlipVerticallyOnLoad(const bool Flip);

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(LTexture, TextureType, TextureFiles, bHasAlpha)

protected:
	virtual void SetupTexture();

	std::vector<std::string> TextureFiles;
	ETextureType TextureType;
	bool bHasAlpha;

	static const char* TextureDirectory;

private:
	unsigned int TextureID;
	int Width;
	int Height;
};