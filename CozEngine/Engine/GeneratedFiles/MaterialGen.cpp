#include "Rendering/Material.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


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

        LClassUtilities::RegisterStaticClassProperties(LMaterial::Class,
                                                        Properties,
                                                        sizeof(LMaterial),
                                                        alignof(LMaterial),
                                                        "LMaterial",
                                                        "LResource",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc);
        
        LClassRegister::RegisterObj("LMaterial", LMaterial::Class);
    }
    return LMaterial::Class;
}

void from_json(const nlohmann::json& Json, LMaterial& Object)
{
    LResource& Parent = Object;
    from_json(Json, Parent);
    Object.Shaders = Json["Shaders"];

}

void to_json(nlohmann::json& Json, const LMaterial& Object)
{
    const LResource& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LMaterial";
    Json["Shaders"] = Object.Shaders;

}
