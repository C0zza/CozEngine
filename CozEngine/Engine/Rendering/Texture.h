#pragma once

#include <string>

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
	virtual void Unload() override {};

	void Use(const unsigned int TextureUnit) const;

	ETextureType GetTextureType() const { return TextureType; }

	static void SetFlipVerticallyOnLoad(const bool Flip);

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(LTexture, TextureType, TextureFile, bHasAlpha)

private:
	unsigned int TextureID;
	int Width;
	int Height;

	ETextureType TextureType;
	std::string TextureFile;
	bool bHasAlpha;

	static const char* TextureDirectory;
};

inline void to_json(nlohmann::json& J, const LResourceHandle<LTexture>& ResourceHandle)
{
	LResourceHandleHelper::ToJsonHelper(J, ResourceHandle);
}

inline void from_json(const nlohmann::json& J, LResourceHandle<LTexture>& ResourceHandle)
{
	LResourceHandleHelper::FromJsonHelper(J, ResourceHandle);
}