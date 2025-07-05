#include "ECS/ECSComponents/CameraComponent.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* CCameraComponent::Class = CCameraComponent::StaticClass();

LClass* CCameraComponent::StaticClass()
{
    if(!CCameraComponent::Class)
    {
        CCameraComponent::Class = new LClass();
        assert(CCameraComponent::Class);

        std::vector<LProperty> Properties
        {

        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new CCameraComponent();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) CCameraComponent();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<CCameraComponent*>(Add)->~CCameraComponent();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const CCameraComponent* Object = reinterpret_cast<const CCameraComponent*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                CCameraComponent* Object = reinterpret_cast<CCameraComponent*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(CCameraComponent::Class,
                                                        Properties,
                                                        sizeof(CCameraComponent),
                                                        alignof(CCameraComponent),
                                                        "CCameraComponent",
                                                        "LECSComponent",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("CCameraComponent", CCameraComponent::Class);
    }
    return CCameraComponent::Class;
}

void from_json(const nlohmann::json& Json, CCameraComponent& Object)
{
    LECSComponent& Parent = Object;
    from_json(Json, Parent);

}

void to_json(nlohmann::json& Json, const CCameraComponent& Object)
{
    const LECSComponent& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "CCameraComponent";

}
