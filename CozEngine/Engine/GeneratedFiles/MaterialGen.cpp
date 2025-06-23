#include "Rendering/Material.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"
#include "ResourceManagement/ResourceHandleHelper.h"


LClass* LMaterial::Class = LMaterial::StaticClass();

LClass* LMaterial::StaticClass()
{
    if(!LMaterial::Class)
    {
        LMaterial::Class = new LClass();
        assert(LMaterial::Class);

        std::vector<LProperty> Properties
        {
            {"Shaders", "std::map<EDrawMode, LResourceHandle<LShader>>", MEMBER_OFFSET(LMaterial, Shaders)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                std::map<EDrawMode, LResourceHandle<LShader>>* ShadersPtr = reinterpret_cast<std::map<EDrawMode, LResourceHandle<LShader>>*>(Address + offsetof(LMaterial, Shaders));
                LImGuiPropertyDrawHelpers::DrawProperty("Shaders", *ShadersPtr);
            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LMaterial();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LMaterial* Object = reinterpret_cast<const LMaterial*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LMaterial* Object = reinterpret_cast<LMaterial*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LMaterial::Class,
                                                        Properties,
                                                        sizeof(LMaterial),
                                                        alignof(LMaterial),
                                                        "LMaterial",
                                                        "LResource",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc);
        
        LClassRegister::RegisterObj("LMaterial", LMaterial::Class);
    }
    return LMaterial::Class;
}

void from_json(const nlohmann::json& Json, LMaterial& Object)
{
    LResource& Parent = Object;
    from_json(Json, Parent);
    if(Json.contains("Shaders")) Object.Shaders = Json["Shaders"];

}

void to_json(nlohmann::json& Json, const LMaterial& Object)
{
    const LResource& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LMaterial";
    Json["Shaders"] = Object.Shaders;

}
