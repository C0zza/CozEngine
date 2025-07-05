#include "ECS/ECSComponents/TransformComponent.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"
#include "Misc/GlmSerialization.h"


LClass* CTransformComponent::Class = CTransformComponent::StaticClass();

LClass* CTransformComponent::StaticClass()
{
    if(!CTransformComponent::Class)
    {
        CTransformComponent::Class = new LClass();
        assert(CTransformComponent::Class);

        std::vector<LProperty> Properties
        {
            {"Position", "glm::vec3", MEMBER_OFFSET(CTransformComponent, Position)},
            {"Rotation", "glm::vec3", MEMBER_OFFSET(CTransformComponent, Rotation)},
            {"Scale", "glm::vec3", MEMBER_OFFSET(CTransformComponent, Scale)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                glm::vec3* PositionPtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CTransformComponent, Position));
                LImGuiPropertyDrawHelpers::DrawProperty("Position", *PositionPtr);

                glm::vec3* RotationPtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CTransformComponent, Rotation));
                LImGuiPropertyDrawHelpers::DrawProperty("Rotation", *RotationPtr);

                glm::vec3* ScalePtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CTransformComponent, Scale));
                LImGuiPropertyDrawHelpers::DrawProperty("Scale", *ScalePtr);
            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new CTransformComponent();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) CTransformComponent();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<CTransformComponent*>(Add)->~CTransformComponent();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const CTransformComponent* Object = reinterpret_cast<const CTransformComponent*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                CTransformComponent* Object = reinterpret_cast<CTransformComponent*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(CTransformComponent::Class,
                                                        Properties,
                                                        sizeof(CTransformComponent),
                                                        alignof(CTransformComponent),
                                                        "CTransformComponent",
                                                        "LECSComponent",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("CTransformComponent", CTransformComponent::Class);
    }
    return CTransformComponent::Class;
}

void from_json(const nlohmann::json& Json, CTransformComponent& Object)
{
    LECSComponent& Parent = Object;
    from_json(Json, Parent);
    if(Json.contains("Position")) Object.Position = Json["Position"];
    if(Json.contains("Rotation")) Object.Rotation = Json["Rotation"];
    if(Json.contains("Scale")) Object.Scale = Json["Scale"];

}

void to_json(nlohmann::json& Json, const CTransformComponent& Object)
{
    const LECSComponent& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "CTransformComponent";
    Json["Position"] = Object.Position;
    Json["Rotation"] = Object.Rotation;
    Json["Scale"] = Object.Scale;

}
