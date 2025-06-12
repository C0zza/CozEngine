#include "ECS/ECSComponents/PointLightComponent.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"
#include "Misc/GlmSerialization.h"


LClass* CPointLightComponent::Class = CPointLightComponent::StaticClass();

LClass* CPointLightComponent::StaticClass()
{
    if(!CPointLightComponent::Class)
    {
        CPointLightComponent::Class = new LClass();
        assert(CPointLightComponent::Class);

        std::vector<LProperty> Properties
        {
            {"Position", "glm::vec3", MEMBER_OFFSET(CPointLightComponent, Position)},
            {"Ambient", "glm::vec3", MEMBER_OFFSET(CPointLightComponent, Ambient)},
            {"Diffuse", "glm::vec3", MEMBER_OFFSET(CPointLightComponent, Diffuse)},
            {"Specular", "glm::vec3", MEMBER_OFFSET(CPointLightComponent, Specular)},
            {"Constant", "float", MEMBER_OFFSET(CPointLightComponent, Constant)},
            {"Linear", "float", MEMBER_OFFSET(CPointLightComponent, Linear)},
            {"Quadratic", "float", MEMBER_OFFSET(CPointLightComponent, Quadratic)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                glm::vec3* PositionPtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CPointLightComponent, Position));
                LImGuiPropertyDrawHelpers::DrawProperty("Position", *PositionPtr);

                glm::vec3* AmbientPtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CPointLightComponent, Ambient));
                LImGuiPropertyDrawHelpers::DrawProperty("Ambient", *AmbientPtr);

                glm::vec3* DiffusePtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CPointLightComponent, Diffuse));
                LImGuiPropertyDrawHelpers::DrawProperty("Diffuse", *DiffusePtr);

                glm::vec3* SpecularPtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CPointLightComponent, Specular));
                LImGuiPropertyDrawHelpers::DrawProperty("Specular", *SpecularPtr);

                float* ConstantPtr = reinterpret_cast<float*>(Address + offsetof(CPointLightComponent, Constant));
                LImGuiPropertyDrawHelpers::DrawProperty("Constant", *ConstantPtr);

                float* LinearPtr = reinterpret_cast<float*>(Address + offsetof(CPointLightComponent, Linear));
                LImGuiPropertyDrawHelpers::DrawProperty("Linear", *LinearPtr);

                float* QuadraticPtr = reinterpret_cast<float*>(Address + offsetof(CPointLightComponent, Quadratic));
                LImGuiPropertyDrawHelpers::DrawProperty("Quadratic", *QuadraticPtr);
            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new CPointLightComponent();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const CPointLightComponent* Object = reinterpret_cast<const CPointLightComponent*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                CPointLightComponent* Object = reinterpret_cast<CPointLightComponent*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(CPointLightComponent::Class,
                                                        Properties,
                                                        sizeof(CPointLightComponent),
                                                        alignof(CPointLightComponent),
                                                        "CPointLightComponent",
                                                        "",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc);
        
        LClassRegister::RegisterObj("CPointLightComponent", CPointLightComponent::Class);
    }
    return CPointLightComponent::Class;
}

void from_json(const nlohmann::json& Json, CPointLightComponent& Object)
{

    Object.Position = Json["Position"];
    Object.Ambient = Json["Ambient"];
    Object.Diffuse = Json["Diffuse"];
    Object.Specular = Json["Specular"];
    Object.Constant = Json["Constant"];
    Object.Linear = Json["Linear"];
    Object.Quadratic = Json["Quadratic"];

}

void to_json(nlohmann::json& Json, const CPointLightComponent& Object)
{

    Json["Type"] = "CPointLightComponent";
    Json["Position"] = Object.Position;
    Json["Ambient"] = Object.Ambient;
    Json["Diffuse"] = Object.Diffuse;
    Json["Specular"] = Object.Specular;
    Json["Constant"] = Object.Constant;
    Json["Linear"] = Object.Linear;
    Json["Quadratic"] = Object.Quadratic;

}
