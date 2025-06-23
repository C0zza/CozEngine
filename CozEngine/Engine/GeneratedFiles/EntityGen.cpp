#include "ECS/Entity.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LEntity::Class = LEntity::StaticClass();

LClass* LEntity::StaticClass()
{
    if(!LEntity::Class)
    {
        LEntity::Class = new LClass();
        assert(LEntity::Class);

        std::vector<LProperty> Properties
        {

        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                // TEMP: Override whenever regenerating. Need to add something like an "Abstract" tag to REFL_CLASS() tags.
                return nullptr;
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LEntity* Object = reinterpret_cast<const LEntity*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LEntity* Object = reinterpret_cast<LEntity*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LEntity::Class,
                                                        Properties,
                                                        sizeof(LEntity),
                                                        alignof(LEntity),
                                                        "LEntity",
                                                        "",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc);
        
        LClassRegister::RegisterObj("LEntity", LEntity::Class);
    }
    return LEntity::Class;
}

void from_json(const nlohmann::json& Json, LEntity& Object)
{


}

void to_json(nlohmann::json& Json, const LEntity& Object)
{

    Json["Type"] = "LEntity";

}
