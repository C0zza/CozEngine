#include "Rendering/Materials/ModelMaterial.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LModelMaterial::Class = LModelMaterial::StaticClass();

LClass* LModelMaterial::StaticClass()
{
    if(!LModelMaterial::Class)
    {
        LModelMaterial::Class = new LClass();
        assert(LModelMaterial::Class);

        std::vector<LProperty> Properties
        {
            {"Diffuse", "LResourceHandle<LTexture>", MEMBER_OFFSET(LModelMaterial, Diffuse)},
            {"Specular", "LResourceHandle<LTexture>", MEMBER_OFFSET(LModelMaterial, Specular)},
            {"NormalMap", "LResourceHandle<LTexture>", MEMBER_OFFSET(LModelMaterial, NormalMap)},
            {"SpecularShininess", "float", MEMBER_OFFSET(LModelMaterial, SpecularShininess)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                LResourceHandle<LTexture>* DiffusePtr = reinterpret_cast<LResourceHandle<LTexture>*>(Address + offsetof(LModelMaterial, Diffuse));
                LImGuiPropertyDrawHelpers::DrawProperty("Diffuse", *DiffusePtr);

                LResourceHandle<LTexture>* SpecularPtr = reinterpret_cast<LResourceHandle<LTexture>*>(Address + offsetof(LModelMaterial, Specular));
                LImGuiPropertyDrawHelpers::DrawProperty("Specular", *SpecularPtr);

                LResourceHandle<LTexture>* NormalMapPtr = reinterpret_cast<LResourceHandle<LTexture>*>(Address + offsetof(LModelMaterial, NormalMap));
                LImGuiPropertyDrawHelpers::DrawProperty("NormalMap", *NormalMapPtr);


            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LModelMaterial();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LModelMaterial* Object = reinterpret_cast<const LModelMaterial*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LModelMaterial* Object = reinterpret_cast<LModelMaterial*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LModelMaterial::Class,
                                                        Properties,
                                                        sizeof(LModelMaterial),
                                                        alignof(LModelMaterial),
                                                        "LModelMaterial",
                                                        "LMaterial",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc);
        
        LClassRegister::RegisterObj("LModelMaterial", LModelMaterial::Class);
    }
    return LModelMaterial::Class;
}

void from_json(const nlohmann::json& Json, LModelMaterial& Object)
{
    LMaterial& Parent = Object;
    from_json(Json, Parent);
    Object.Diffuse = Json["Diffuse"];
    Object.Specular = Json["Specular"];
    Object.NormalMap = Json["NormalMap"];
    Object.SpecularShininess = Json["SpecularShininess"];

}

void to_json(nlohmann::json& Json, const LModelMaterial& Object)
{
    const LMaterial& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LModelMaterial";
    Json["Diffuse"] = Object.Diffuse;
    Json["Specular"] = Object.Specular;
    Json["NormalMap"] = Object.NormalMap;
    Json["SpecularShininess"] = Object.SpecularShininess;

}
