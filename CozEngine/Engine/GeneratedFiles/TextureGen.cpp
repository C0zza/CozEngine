#include "Rendering/Texture.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LTexture::Class = LTexture::StaticClass();

LClass* LTexture::StaticClass()
{
    if(!LTexture::Class)
    {
        LTexture::Class = new LClass();
        assert(LTexture::Class);

        std::vector<LProperty> Properties
        {
            {"TextureFiles", "std::vector<std::string>", MEMBER_OFFSET(LTexture, TextureFiles)},
            {"TextureType", "ETextureType", MEMBER_OFFSET(LTexture, TextureType)},
            {"bHasAlpha", "bool", MEMBER_OFFSET(LTexture, bHasAlpha)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                std::vector<std::string>* TextureFilesPtr = reinterpret_cast<std::vector<std::string>*>(Address + offsetof(LTexture, TextureFiles));
                LImGuiPropertyDrawHelpers::DrawProperty("TextureFiles", *TextureFilesPtr);

                ETextureType* TextureTypePtr = reinterpret_cast<ETextureType*>(Address + offsetof(LTexture, TextureType));
                LImGuiPropertyDrawHelpers::DrawProperty("TextureType", *TextureTypePtr);

                bool* bHasAlphaPtr = reinterpret_cast<bool*>(Address + offsetof(LTexture, bHasAlpha));
                LImGuiPropertyDrawHelpers::DrawProperty("bHasAlpha", *bHasAlphaPtr);
            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LTexture();
            };

        LClassUtilities::RegisterStaticClassProperties(LTexture::Class,
                                                        Properties,
                                                        sizeof(LTexture),
                                                        alignof(LTexture),
                                                        "LTexture",
                                                        "LResource",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc);
        
        LClassRegister::RegisterObj("LTexture", LTexture::Class);
    }
    return LTexture::Class;
}

void from_json(const nlohmann::json& Json, LTexture& Object)
{
    LResource& Parent = Object;
    from_json(Json, Parent);
    Object.TextureFiles = Json["TextureFiles"];
    Object.TextureType = Json["TextureType"];
    Object.bHasAlpha = Json["bHasAlpha"];

}

void to_json(nlohmann::json& Json, const LTexture& Object)
{
    const LResource& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LTexture";
    Json["TextureFiles"] = Object.TextureFiles;
    Json["TextureType"] = Object.TextureType;
    Json["bHasAlpha"] = Object.bHasAlpha;

}
