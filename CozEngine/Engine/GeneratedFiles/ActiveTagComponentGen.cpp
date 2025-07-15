#include "ECS/ECSComponents/ActiveTagComponent.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LActiveTagComponent::Class = LActiveTagComponent::StaticClass();

LClass* LActiveTagComponent::StaticClass()
{
    if(!LActiveTagComponent::Class)
    {
        LActiveTagComponent::Class = new LClass();
        assert(LActiveTagComponent::Class);

        std::vector<LProperty> Properties
        {

        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LActiveTagComponent();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) LActiveTagComponent();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<LActiveTagComponent*>(Add)->~LActiveTagComponent();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LActiveTagComponent* Object = reinterpret_cast<const LActiveTagComponent*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LActiveTagComponent* Object = reinterpret_cast<LActiveTagComponent*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LActiveTagComponent::Class,
                                                        Properties,
                                                        sizeof(LActiveTagComponent),
                                                        alignof(LActiveTagComponent),
                                                        "LActiveTagComponent",
                                                        "LECSComponent",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LActiveTagComponent", LActiveTagComponent::Class);
    }
    return LActiveTagComponent::Class;
}

void from_json(const nlohmann::json& Json, LActiveTagComponent& Object)
{
    LECSComponent& Parent = Object;
    from_json(Json, Parent);

}

void to_json(nlohmann::json& Json, const LActiveTagComponent& Object)
{
    const LECSComponent& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LActiveTagComponent";

}
