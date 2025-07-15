#include "ECS/ECSComponents/Lighting/DirectionalLightProcessor.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LDirectionalLightProcessor::Class = LDirectionalLightProcessor::StaticClass();

LClass* LDirectionalLightProcessor::StaticClass()
{
    if(!LDirectionalLightProcessor::Class)
    {
        LDirectionalLightProcessor::Class = new LClass();
        assert(LDirectionalLightProcessor::Class);

        std::vector<LProperty> Properties
        {

        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LDirectionalLightProcessor();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) LDirectionalLightProcessor();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<LDirectionalLightProcessor*>(Add)->~LDirectionalLightProcessor();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LDirectionalLightProcessor* Object = reinterpret_cast<const LDirectionalLightProcessor*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LDirectionalLightProcessor* Object = reinterpret_cast<LDirectionalLightProcessor*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LDirectionalLightProcessor::Class,
                                                        Properties,
                                                        sizeof(LDirectionalLightProcessor),
                                                        alignof(LDirectionalLightProcessor),
                                                        "LDirectionalLightProcessor",
                                                        "LEntityProcessor",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LDirectionalLightProcessor", LDirectionalLightProcessor::Class);
    }
    return LDirectionalLightProcessor::Class;
}

void from_json(const nlohmann::json& Json, LDirectionalLightProcessor& Object)
{
    LEntityProcessor& Parent = Object;
    from_json(Json, Parent);

}

void to_json(nlohmann::json& Json, const LDirectionalLightProcessor& Object)
{
    const LEntityProcessor& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LDirectionalLightProcessor";

}
