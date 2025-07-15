#include "Components/MovementProcessor.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LMovementProcessor::Class = LMovementProcessor::StaticClass();

LClass* LMovementProcessor::StaticClass()
{
    if(!LMovementProcessor::Class)
    {
        LMovementProcessor::Class = new LClass();
        assert(LMovementProcessor::Class);

        std::vector<LProperty> Properties
        {

        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LMovementProcessor();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) LMovementProcessor();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<LMovementProcessor*>(Add)->~LMovementProcessor();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LMovementProcessor* Object = reinterpret_cast<const LMovementProcessor*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LMovementProcessor* Object = reinterpret_cast<LMovementProcessor*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LMovementProcessor::Class,
                                                        Properties,
                                                        sizeof(LMovementProcessor),
                                                        alignof(LMovementProcessor),
                                                        "LMovementProcessor",
                                                        "LEntityProcessor",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LMovementProcessor", LMovementProcessor::Class);
    }
    return LMovementProcessor::Class;
}

void from_json(const nlohmann::json& Json, LMovementProcessor& Object)
{
    LEntityProcessor& Parent = Object;
    from_json(Json, Parent);

}

void to_json(nlohmann::json& Json, const LMovementProcessor& Object)
{
    const LEntityProcessor& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LMovementProcessor";

}
