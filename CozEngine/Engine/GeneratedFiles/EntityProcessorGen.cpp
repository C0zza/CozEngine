#include "ECS/ECS2/EntityProcessor.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LEntityProcessor::Class = LEntityProcessor::StaticClass();

LClass* LEntityProcessor::StaticClass()
{
    if(!LEntityProcessor::Class)
    {
        LEntityProcessor::Class = new LClass();
        assert(LEntityProcessor::Class);

        std::vector<LProperty> Properties
        {

        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return nullptr;
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LEntityProcessor* Object = reinterpret_cast<const LEntityProcessor*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LEntityProcessor* Object = reinterpret_cast<LEntityProcessor*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LEntityProcessor::Class,
                                                        Properties,
                                                        sizeof(LEntityProcessor),
                                                        alignof(LEntityProcessor),
                                                        "LEntityProcessor",
                                                        "",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LEntityProcessor", LEntityProcessor::Class);
    }
    return LEntityProcessor::Class;
}

void from_json(const nlohmann::json& Json, LEntityProcessor& Object)
{


}

void to_json(nlohmann::json& Json, const LEntityProcessor& Object)
{

    Json["Type"] = "LEntityProcessor";

}
