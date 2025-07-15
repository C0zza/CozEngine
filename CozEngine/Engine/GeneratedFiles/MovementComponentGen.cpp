#include "Components/MovementComponent.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"
#include "Misc/GlmSerialization.h"


LClass* CMovementComponent::Class = CMovementComponent::StaticClass();

LClass* CMovementComponent::StaticClass()
{
    if(!CMovementComponent::Class)
    {
        CMovementComponent::Class = new LClass();
        assert(CMovementComponent::Class);

        std::vector<LProperty> Properties
        {
            {"Movement", "glm::vec3", MEMBER_OFFSET(CMovementComponent, Movement)},
            {"Speed", "float", MEMBER_OFFSET(CMovementComponent, Speed)},
            {"Rotation", "glm::vec3", MEMBER_OFFSET(CMovementComponent, Rotation)},
            {"Sensitivity", "float", MEMBER_OFFSET(CMovementComponent, Sensitivity)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                glm::vec3* MovementPtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CMovementComponent, Movement));
                LImGuiPropertyDrawHelpers::DrawProperty("Movement", *MovementPtr);

                float* SpeedPtr = reinterpret_cast<float*>(Address + offsetof(CMovementComponent, Speed));
                LImGuiPropertyDrawHelpers::DrawProperty("Speed", *SpeedPtr);

                glm::vec3* RotationPtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CMovementComponent, Rotation));
                LImGuiPropertyDrawHelpers::DrawProperty("Rotation", *RotationPtr);

                float* SensitivityPtr = reinterpret_cast<float*>(Address + offsetof(CMovementComponent, Sensitivity));
                LImGuiPropertyDrawHelpers::DrawProperty("Sensitivity", *SensitivityPtr);
            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new CMovementComponent();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) CMovementComponent();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<CMovementComponent*>(Add)->~CMovementComponent();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const CMovementComponent* Object = reinterpret_cast<const CMovementComponent*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                CMovementComponent* Object = reinterpret_cast<CMovementComponent*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(CMovementComponent::Class,
                                                        Properties,
                                                        sizeof(CMovementComponent),
                                                        alignof(CMovementComponent),
                                                        "CMovementComponent",
                                                        "LECSComponent",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("CMovementComponent", CMovementComponent::Class);
    }
    return CMovementComponent::Class;
}

void from_json(const nlohmann::json& Json, CMovementComponent& Object)
{
    LECSComponent& Parent = Object;
    from_json(Json, Parent);
    if(Json.contains("Movement")) Object.Movement = Json["Movement"];
    if(Json.contains("Speed")) Object.Speed = Json["Speed"];
    if(Json.contains("Rotation")) Object.Rotation = Json["Rotation"];
    if(Json.contains("Sensitivity")) Object.Sensitivity = Json["Sensitivity"];

}

void to_json(nlohmann::json& Json, const CMovementComponent& Object)
{
    const LECSComponent& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "CMovementComponent";
    Json["Movement"] = Object.Movement;
    Json["Speed"] = Object.Speed;
    Json["Rotation"] = Object.Rotation;
    Json["Sensitivity"] = Object.Sensitivity;

}
