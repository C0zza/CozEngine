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

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) CSpotLightComponent();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<CSpotLightComponent*>(Add)->~CSpotLightComponent();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const CSpotLightComponent* Object = reinterpret_cast<const CSpotLightComponent*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                CSpotLightComponent* Object = reinterpret_cast<CSpotLightComponent*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(CSpotLightComponent::Class,
                                                        Properties,
                                                        sizeof(CSpotLightComponent),
                                                        alignof(CSpotLightComponent),
                                                        "CSpotLightComponent",
                                                        "LECSComponent",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("CSpotLightComponent", CSpotLightComponent::Class);
    }
    return CSpotLightComponent::Class;
}

void from_json(const nlohmann::json& Json, CSpotLightComponent& Object)
{
    LECSComponent& Parent = Object;
    from_json(Json, Parent);
    if(Json.contains("Position")) Object.Position = Json["Position"];
    if(Json.contains("Direction")) Object.Direction = Json["Direction"];
    if(Json.contains("Ambient")) Object.Ambient = Json["Ambient"];
    if(Json.contains("Diffuse")) Object.Diffuse = Json["Diffuse"];
    if(Json.contains("Specular")) Object.Specular = Json["Specular"];
    if(Json.contains("CutOff")) Object.CutOff = Json["CutOff"];
    if(Json.contains("OuterCutOff")) Object.OuterCutOff = Json["OuterCutOff"];
    if(Json.contains("Constant")) Object.Constant = Json["Constant"];
    if(Json.contains("Linear")) Object.Linear = Json["Linear"];
    if(Json.contains("Quadratic")) Object.Quadratic = Json["Quadratic"];

}

void to_json(nlohmann::json& Json, const CSpotLightComponent& Object)
{
    const LECSComponent& Parent = Object;
    to_json(Json, Parent);
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
