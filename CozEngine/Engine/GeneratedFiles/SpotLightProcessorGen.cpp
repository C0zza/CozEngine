#include "ECS/ECSComponents/Lighting/SpotLightProcessor.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LSpotLightProcessor::Class = LSpotLightProcessor::StaticClass();

LClass* LSpotLightProcessor::StaticClass()
{
    if(!LSpotLightProcessor::Class)
    {
        LSpotLightProcessor::Class = new LClass();
        assert(LSpotLightProcessor::Class);

        std::vector<LProperty> Properties
        {

        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LSpotLightProcessor();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) LSpotLightProcessor();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<LSpotLightProcessor*>(Add)->~LSpotLightProcessor();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LSpotLightProcessor* Object = reinterpret_cast<const LSpotLightProcessor*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LSpotLightProcessor* Object = reinterpret_cast<LSpotLightProcessor*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LSpotLightProcessor::Class,
                                                        Properties,
                                                        sizeof(LSpotLightProcessor),
                                                        alignof(LSpotLightProcessor),
                                                        "LSpotLightProcessor",
                                                        "LEntityProcessor",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LSpotLightProcessor", LSpotLightProcessor::Class);
    }
    return LSpotLightProcessor::Class;
}

void from_json(const nlohmann::json& Json, LSpotLightProcessor& Object)
{
    LEntityProcessor& Parent = Object;
    from_json(Json, Parent);

}

void to_json(nlohmann::json& Json, const LSpotLightProcessor& Object)
{
    const LEntityProcessor& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LSpotLightProcessor";

}
