#include "Rendering/Materials/LandscapeMaterial.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LLandscapeMaterial::Class = LLandscapeMaterial::StaticClass();

LClass* LLandscapeMaterial::StaticClass()
{
    if(!LLandscapeMaterial::Class)
    {
        LLandscapeMaterial::Class = new LClass();
        assert(LLandscapeMaterial::Class);

        std::vector<LProperty> Properties
        {
            {"HeightMap", "LResourceHandle<LTexture>", MEMBER_OFFSET(LLandscapeMaterial, HeightMap)},
            {"GroundTexture", "LResourceHandle<LTexture>", MEMBER_OFFSET(LLandscapeMaterial, GroundTexture)},
            {"WallTexture", "LResourceHandle<LTexture>", MEMBER_OFFSET(LLandscapeMaterial, WallTexture)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                LResourceHandle<LTexture>* HeightMapPtr = reinterpret_cast<LResourceHandle<LTexture>*>(Address + offsetof(LLandscapeMaterial, HeightMap));
                LImGuiPropertyDrawHelpers::DrawProperty("HeightMap", *HeightMapPtr);

                LResourceHandle<LTexture>* GroundTexturePtr = reinterpret_cast<LResourceHandle<LTexture>*>(Address + offsetof(LLandscapeMaterial, GroundTexture));
                LImGuiPropertyDrawHelpers::DrawProperty("GroundTexture", *GroundTexturePtr);

                LResourceHandle<LTexture>* WallTexturePtr = reinterpret_cast<LResourceHandle<LTexture>*>(Address + offsetof(LLandscapeMaterial, WallTexture));
                LImGuiPropertyDrawHelpers::DrawProperty("WallTexture", *WallTexturePtr);
            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LLandscapeMaterial();
            };

        LClassUtilities::RegisterStaticClassProperties(LLandscapeMaterial::Class,
                                                        Properties,
                                                        sizeof(LLandscapeMaterial),
                                                        alignof(LLandscapeMaterial),
                                                        "LLandscapeMaterial",
                                                        "LMaterial",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc);
        
        LClassRegister::RegisterObj("LLandscapeMaterial", LLandscapeMaterial::Class);
    }
    return LLandscapeMaterial::Class;
}

void from_json(const nlohmann::json& Json, LLandscapeMaterial& Object)
{
    LMaterial& Parent = Object;
    from_json(Json, Parent);
    Object.HeightMap = Json["HeightMap"];
    Object.GroundTexture = Json["GroundTexture"];
    Object.WallTexture = Json["WallTexture"];

}

void to_json(nlohmann::json& Json, const LLandscapeMaterial& Object)
{
    const LMaterial& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LLandscapeMaterial";
    Json["HeightMap"] = Object.HeightMap;
    Json["GroundTexture"] = Object.GroundTexture;
    Json["WallTexture"] = Object.WallTexture;

}
