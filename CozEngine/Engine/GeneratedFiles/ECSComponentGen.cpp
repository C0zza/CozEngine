#include "ECS/ECSComponents/ECSComponent.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LECSComponent::Class = LECSComponent::StaticClass();

LClass* LECSComponent::StaticClass()
{
    if(!LECSComponent::Class)
    {
        LECSComponent::Class = new LClass();
        assert(LECSComponent::Class);

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
                const LECSComponent* Object = reinterpret_cast<const LECSComponent*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LECSComponent* Object = reinterpret_cast<LECSComponent*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LECSComponent::Class,
                                                        Properties,
                                                        sizeof(LECSComponent),
                                                        alignof(LECSComponent),
                                                        "LECSComponent",
                                                        "",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LECSComponent", LECSComponent::Class);
    }
    return LECSComponent::Class;
}

void from_json(const nlohmann::json& Json, LECSComponent& Object)
{


}

void to_json(nlohmann::json& Json, const LECSComponent& Object)
{

    Json["Type"] = "LECSComponent";

}
