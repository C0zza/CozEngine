#include "ECS/ECSComponents/LandscapeComponent.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"
#include "ResourceManagement/ResourceHandleHelper.h"


LClass* CLandscapeComponent::Class = CLandscapeComponent::StaticClass();

LClass* CLandscapeComponent::StaticClass()
{
    if(!CLandscapeComponent::Class)
    {
        CLandscapeComponent::Class = new LClass();
        assert(CLandscapeComponent::Class);

        std::vector<LProperty> Properties
        {
            {"LandscapeMaterial", "LResourceHandle<LLandscapeMaterial>", MEMBER_OFFSET(CLandscapeComponent, LandscapeMaterial)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                LResourceHandle<LLandscapeMaterial>* LandscapeMaterialPtr = reinterpret_cast<LResourceHandle<LLandscapeMaterial>*>(Address + offsetof(CLandscapeComponent, LandscapeMaterial));
                LImGuiPropertyDrawHelpers::DrawProperty("LandscapeMaterial", *LandscapeMaterialPtr);
            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new CLandscapeComponent();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) CLandscapeComponent();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<CLandscapeComponent*>(Add)->~CLandscapeComponent();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const CLandscapeComponent* Object = reinterpret_cast<const CLandscapeComponent*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                CLandscapeComponent* Object = reinterpret_cast<CLandscapeComponent*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(CLandscapeComponent::Class,
                                                        Properties,
                                                        sizeof(CLandscapeComponent),
                                                        alignof(CLandscapeComponent),
                                                        "CLandscapeComponent",
                                                        "LECSComponent",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("CLandscapeComponent", CLandscapeComponent::Class);
    }
    return CLandscapeComponent::Class;
}

void from_json(const nlohmann::json& Json, CLandscapeComponent& Object)
{
    LECSComponent& Parent = Object;
    from_json(Json, Parent);
    if(Json.contains("LandscapeMaterial")) Object.LandscapeMaterial = Json["LandscapeMaterial"];

}

void to_json(nlohmann::json& Json, const CLandscapeComponent& Object)
{
    const LECSComponent& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "CLandscapeComponent";
    Json["LandscapeMaterial"] = Object.LandscapeMaterial;

}
