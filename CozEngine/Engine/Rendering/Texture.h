#pragma once

#include <string>
#include <vector>

#include "Reflection/Reflection.h"
#include "ResourceManagement/ResourceHandleHelper.h"

enum class ETextureType
{
	Diffuse,
	Specular
};

REFL_CLASS()
class LTexture : public LResource
{
	REFL_GENERATED_BODY(LTexture)
public:
	virtual void Load() override;
	virtual void Unload() override;

	void Use(const unsigned int TextureUnit) const;

	unsigned int GetTextureID() const { return TextureID; }
	ETextureType GetTextureType() const { return TextureType; }
	int GetWidth() const { return Width; }
	int GetHeight() const { return Height; }

	static void SetFlipVerticallyOnLoad(const bool Flip);

protected:
	virtual void SetupTexture();

	REFL_PROP(Visible)
	std::vector<std::string> TextureFiles;
	REFL_PROP(Visible)
	ETextureType TextureType;
	REFL_PROP(Visible)
	bool bHasAlpha;

private:
	unsigned int TextureID;
	int Width;
	int Height;
};