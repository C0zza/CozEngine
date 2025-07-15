#include "ECS/ECSComponents/Lighting/PointLightProcessor.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LPointLightProcessor::Class = LPointLightProcessor::StaticClass();

LClass* LPointLightProcessor::StaticClass()
{
    if(!LPointLightProcessor::Class)
    {
        LPointLightProcessor::Class = new LClass();
        assert(LPointLightProcessor::Class);

        std::vector<LProperty> Properties
        {

        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LPointLightProcessor();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) LPointLightProcessor();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<LPointLightProcessor*>(Add)->~LPointLightProcessor();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LPointLightProcessor* Object = reinterpret_cast<const LPointLightProcessor*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LPointLightProcessor* Object = reinterpret_cast<LPointLightProcessor*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LPointLightProcessor::Class,
                                                        Properties,
                                                        sizeof(LPointLightProcessor),
                                                        alignof(LPointLightProcessor),
                                                        "LPointLightProcessor",
                                                        "LEntityProcessor",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LPointLightProcessor", LPointLightProcessor::Class);
    }
    return LPointLightProcessor::Class;
}

void from_json(const nlohmann::json& Json, LPointLightProcessor& Object)
{
    LEntityProcessor& Parent = Object;
    from_json(Json, Parent);

}

void to_json(nlohmann::json& Json, const LPointLightProcessor& Object)
{
    const LEntityProcessor& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LPointLightProcessor";

}
