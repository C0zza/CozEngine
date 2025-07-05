#include "ECS/ECS2/ArchetypeConfig.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LArchetypeConfig::Class = LArchetypeConfig::StaticClass();

LClass* LArchetypeConfig::StaticClass()
{
    if(!LArchetypeConfig::Class)
    {
        LArchetypeConfig::Class = new LClass();
        assert(LArchetypeConfig::Class);

        std::vector<LProperty> Properties
        {
            {"ComponentSet", "std::set<TSubclassOf<LECSComponent>>", MEMBER_OFFSET(LArchetypeConfig, ComponentSet)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                std::set<TSubclassOf<LECSComponent>>* ComponentSetPtr = reinterpret_cast<std::set<TSubclassOf<LECSComponent>>*>(Address + offsetof(LArchetypeConfig, ComponentSet));
                LImGuiPropertyDrawHelpers::DrawProperty("ComponentSet", *ComponentSetPtr);
            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LArchetypeConfig();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) LArchetypeConfig();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<LArchetypeConfig*>(Add)->~LArchetypeConfig();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LArchetypeConfig* Object = reinterpret_cast<const LArchetypeConfig*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LArchetypeConfig* Object = reinterpret_cast<LArchetypeConfig*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LArchetypeConfig::Class,
                                                        Properties,
                                                        sizeof(LArchetypeConfig),
                                                        alignof(LArchetypeConfig),
                                                        "LArchetypeConfig",
                                                        "LResource",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LArchetypeConfig", LArchetypeConfig::Class);
    }
    return LArchetypeConfig::Class;
}

void from_json(const nlohmann::json& Json, LArchetypeConfig& Object)
{
    LResource& Parent = Object;
    from_json(Json, Parent);
    if(Json.contains("ComponentSet")) Object.ComponentSet = Json["ComponentSet"];

}

void to_json(nlohmann::json& Json, const LArchetypeConfig& Object)
{
    const LResource& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LArchetypeConfig";
    Json["ComponentSet"] = Object.ComponentSet;

}
