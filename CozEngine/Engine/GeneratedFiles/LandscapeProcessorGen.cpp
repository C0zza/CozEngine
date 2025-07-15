#include "ECS/ECSComponents/LandscapeProcessor.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LLandscapeProcessor::Class = LLandscapeProcessor::StaticClass();

LClass* LLandscapeProcessor::StaticClass()
{
    if(!LLandscapeProcessor::Class)
    {
        LLandscapeProcessor::Class = new LClass();
        assert(LLandscapeProcessor::Class);

        std::vector<LProperty> Properties
        {

        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LLandscapeProcessor();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) LLandscapeProcessor();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<LLandscapeProcessor*>(Add)->~LLandscapeProcessor();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LLandscapeProcessor* Object = reinterpret_cast<const LLandscapeProcessor*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LLandscapeProcessor* Object = reinterpret_cast<LLandscapeProcessor*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LLandscapeProcessor::Class,
                                                        Properties,
                                                        sizeof(LLandscapeProcessor),
                                                        alignof(LLandscapeProcessor),
                                                        "LLandscapeProcessor",
                                                        "LEntityProcessor",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LLandscapeProcessor", LLandscapeProcessor::Class);
    }
    return LLandscapeProcessor::Class;
}

void from_json(const nlohmann::json& Json, LLandscapeProcessor& Object)
{
    LEntityProcessor& Parent = Object;
    from_json(Json, Parent);

}

void to_json(nlohmann::json& Json, const LLandscapeProcessor& Object)
{
    const LEntityProcessor& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LLandscapeProcessor";

}
