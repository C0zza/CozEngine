#include "ECS/ECSComponents/Model/ModelProcessor.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LModelProcessor::Class = LModelProcessor::StaticClass();

LClass* LModelProcessor::StaticClass()
{
    if(!LModelProcessor::Class)
    {
        LModelProcessor::Class = new LClass();
        assert(LModelProcessor::Class);

        std::vector<LProperty> Properties
        {

        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LModelProcessor();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) LModelProcessor();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<LModelProcessor*>(Add)->~LModelProcessor();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LModelProcessor* Object = reinterpret_cast<const LModelProcessor*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LModelProcessor* Object = reinterpret_cast<LModelProcessor*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LModelProcessor::Class,
                                                        Properties,
                                                        sizeof(LModelProcessor),
                                                        alignof(LModelProcessor),
                                                        "LModelProcessor",
                                                        "LEntityProcessor",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LModelProcessor", LModelProcessor::Class);
    }
    return LModelProcessor::Class;
}

void from_json(const nlohmann::json& Json, LModelProcessor& Object)
{
    LEntityProcessor& Parent = Object;
    from_json(Json, Parent);

}

void to_json(nlohmann::json& Json, const LModelProcessor& Object)
{
    const LEntityProcessor& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LModelProcessor";

}
