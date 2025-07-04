#include "Rendering/Materials/LandscapeMaterial.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"
#include "ResourceManagement/ResourceHandleHelper.h"


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

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) LLandscapeMaterial();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<LLandscapeMaterial*>(Add)->~LLandscapeMaterial();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LLandscapeMaterial* Object = reinterpret_cast<const LLandscapeMaterial*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LLandscapeMaterial* Object = reinterpret_cast<LLandscapeMaterial*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LLandscapeMaterial::Class,
                                                        Properties,
                                                        sizeof(LLandscapeMaterial),
                                                        alignof(LLandscapeMaterial),
                                                        "LLandscapeMaterial",
                                                        "LMaterial",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LLandscapeMaterial", LLandscapeMaterial::Class);
    }
    return LLandscapeMaterial::Class;
}

void from_json(const nlohmann::json& Json, LLandscapeMaterial& Object)
{
    LMaterial& Parent = Object;
    from_json(Json, Parent);
    if(Json.contains("HeightMap")) Object.HeightMap = Json["HeightMap"];
    if(Json.contains("GroundTexture")) Object.GroundTexture = Json["GroundTexture"];
    if(Json.contains("WallTexture")) Object.WallTexture = Json["WallTexture"];

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
