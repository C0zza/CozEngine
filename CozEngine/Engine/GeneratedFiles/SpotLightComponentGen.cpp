#include "ECS/ECSComponents/SpotLightComponent.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"
#include "Misc/GlmSerialization.h"


LClass* CSpotLightComponent::Class = CSpotLightComponent::StaticClass();

LClass* CSpotLightComponent::StaticClass()
{
    if(!CSpotLightComponent::Class)
    {
        CSpotLightComponent::Class = new LClass();
        assert(CSpotLightComponent::Class);

        std::vector<LProperty> Properties
        {
            {"Position", "glm::vec3", MEMBER_OFFSET(CSpotLightComponent, Position)},
            {"Direction", "glm::vec3", MEMBER_OFFSET(CSpotLightComponent, Direction)},
            {"Ambient", "glm::vec3", MEMBER_OFFSET(CSpotLightComponent, Ambient)},
            {"Diffuse", "glm::vec3", MEMBER_OFFSET(CSpotLightComponent, Diffuse)},
            {"Specular", "glm::vec3", MEMBER_OFFSET(CSpotLightComponent, Specular)},
            {"CutOff", "float", MEMBER_OFFSET(CSpotLightComponent, CutOff)},
            {"OuterCutOff", "float", MEMBER_OFFSET(CSpotLightComponent, OuterCutOff)},
            {"Constant", "float", MEMBER_OFFSET(CSpotLightComponent, Constant)},
            {"Linear", "float", MEMBER_OFFSET(CSpotLightComponent, Linear)},
            {"Quadratic", "float", MEMBER_OFFSET(CSpotLightComponent, Quadratic)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                glm::vec3* PositionPtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CSpotLightComponent, Position));
                LImGuiPropertyDrawHelpers::DrawProperty("Position", *PositionPtr);

                glm::vec3* DirectionPtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CSpotLightComponent, Direction));
                LImGuiPropertyDrawHelpers::DrawProperty("Direction", *DirectionPtr);

                glm::vec3* AmbientPtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CSpotLightComponent, Ambient));
                LImGuiPropertyDrawHelpers::DrawProperty("Ambient", *AmbientPtr);

                glm::vec3* DiffusePtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CSpotLightComponent, Diffuse));
                LImGuiPropertyDrawHelpers::DrawProperty("Diffuse", *DiffusePtr);

                glm::vec3* SpecularPtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CSpotLightComponent, Specular));
                LImGuiPropertyDrawHelpers::DrawProperty("Specular", *SpecularPtr);

                float* CutOffPtr = reinterpret_cast<float*>(Address + offsetof(CSpotLightComponent, CutOff));
                LImGuiPropertyDrawHelpers::DrawProperty("CutOff", *CutOffPtr);

                float* OuterCutOffPtr = reinterpret_cast<float*>(Address + offsetof(CSpotLightComponent, OuterCutOff));
                LImGuiPropertyDrawHelpers::DrawProperty("OuterCutOff", *OuterCutOffPtr);

                float* ConstantPtr = reinterpret_cast<float*>(Address + offsetof(CSpotLightComponent, Constant));
                LImGuiPropertyDrawHelpers::DrawProperty("Constant", *ConstantPtr);

                float* LinearPtr = reinterpret_cast<float*>(Address + offsetof(CSpotLightComponent, Linear));
                LImGuiPropertyDrawHelpers::DrawProperty("Linear", *LinearPtr);

                float* QuadraticPtr = reinterpret_cast<float*>(Address + offsetof(CSpotLightComponent, Quadratic));
                LImGuiPropertyDrawHelpers::DrawProperty("Quadratic", *QuadraticPtr);
            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new CSpotLightComponent();
            };

        LClassUtilities::RegisterStaticClassProperties(CSpotLightComponent::Class,
                                                        Properties,
                                                        sizeof(CSpotLightComponent),
                                                        alignof(CSpotLightComponent),
                                                        "CSpotLightComponent",
                                                        "",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc);
        
        LClassRegister::RegisterObj("CSpotLightComponent", CSpotLightComponent::Class);
    }
    return CSpotLightComponent::Class;
}

void from_json(const nlohmann::json& Json, CSpotLightComponent& Object)
{

    Object.Position = Json["Position"];
    Object.Direction = Json["Direction"];
    Object.Ambient = Json["Ambient"];
    Object.Diffuse = Json["Diffuse"];
    Object.Specular = Json["Specular"];
    Object.CutOff = Json["CutOff"];
    Object.OuterCutOff = Json["OuterCutOff"];
    Object.Constant = Json["Constant"];
    Object.Linear = Json["Linear"];
    Object.Quadratic = Json["Quadratic"];

}

void to_json(nlohmann::json& Json, const CSpotLightComponent& Object)
{

    Json["Type"] = "CSpotLightComponent";
    Json["Position"] = Object.Position;
    Json["Direction"] = Object.Direction;
    Json["Ambient"] = Object.Ambient;
    Json["Diffuse"] = Object.Diffuse;
    Json["Specular"] = Object.Specular;
    Json["CutOff"] = Object.CutOff;
    Json["OuterCutOff"] = Object.OuterCutOff;
    Json["Constant"] = Object.Constant;
    Json["Linear"] = Object.Linear;
    Json["Quadratic"] = Object.Quadratic;

}
