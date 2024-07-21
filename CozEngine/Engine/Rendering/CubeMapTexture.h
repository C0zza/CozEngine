#pragma once

#include "Rendering/Texture.h"

class LCubeMapTexture : public LTexture
{
protected:
	virtual void SetupTexture() override;
};

