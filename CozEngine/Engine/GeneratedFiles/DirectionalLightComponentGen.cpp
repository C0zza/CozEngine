#include "ECS/ECSComponents/DirectionalLightComponent.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"
#include "Misc/GlmSerialization.h"


LClass* CDirectionalLightComponent::Class = CDirectionalLightComponent::StaticClass();

LClass* CDirectionalLightComponent::StaticClass()
{
    if(!CDirectionalLightComponent::Class)
    {
        CDirectionalLightComponent::Class = new LClass();
        assert(CDirectionalLightComponent::Class);

        std::vector<LProperty> Properties
        {
            {"Direction", "glm::vec3", MEMBER_OFFSET(CDirectionalLightComponent, Direction)},
            {"Ambient", "glm::vec3", MEMBER_OFFSET(CDirectionalLightComponent, Ambient)},
            {"Diffuse", "glm::vec3", MEMBER_OFFSET(CDirectionalLightComponent, Diffuse)},
            {"Specular", "glm::vec3", MEMBER_OFFSET(CDirectionalLightComponent, Specular)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                glm::vec3* DirectionPtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CDirectionalLightComponent, Direction));
                LImGuiPropertyDrawHelpers::DrawProperty("Direction", *DirectionPtr);

                glm::vec3* AmbientPtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CDirectionalLightComponent, Ambient));
                LImGuiPropertyDrawHelpers::DrawProperty("Ambient", *AmbientPtr);

                glm::vec3* DiffusePtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CDirectionalLightComponent, Diffuse));
                LImGuiPropertyDrawHelpers::DrawProperty("Diffuse", *DiffusePtr);

                glm::vec3* SpecularPtr = reinterpret_cast<glm::vec3*>(Address + offsetof(CDirectionalLightComponent, Specular));
                LImGuiPropertyDrawHelpers::DrawProperty("Specular", *SpecularPtr);
            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new CDirectionalLightComponent();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) CDirectionalLightComponent();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<CDirectionalLightComponent*>(Add)->~CDirectionalLightComponent();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const CDirectionalLightComponent* Object = reinterpret_cast<const CDirectionalLightComponent*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                CDirectionalLightComponent* Object = reinterpret_cast<CDirectionalLightComponent*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(CDirectionalLightComponent::Class,
                                                        Properties,
                                                        sizeof(CDirectionalLightComponent),
                                                        alignof(CDirectionalLightComponent),
                                                        "CDirectionalLightComponent",
                                                        "LECSComponent",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("CDirectionalLightComponent", CDirectionalLightComponent::Class);
    }
    return CDirectionalLightComponent::Class;
}

void from_json(const nlohmann::json& Json, CDirectionalLightComponent& Object)
{
    LECSComponent& Parent = Object;
    from_json(Json, Parent);
    if(Json.contains("Direction")) Object.Direction = Json["Direction"];
    if(Json.contains("Ambient")) Object.Ambient = Json["Ambient"];
    if(Json.contains("Diffuse")) Object.Diffuse = Json["Diffuse"];
    if(Json.contains("Specular")) Object.Specular = Json["Specular"];

}

void to_json(nlohmann::json& Json, const CDirectionalLightComponent& Object)
{
    const LECSComponent& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "CDirectionalLightComponent";
    Json["Direction"] = Object.Direction;
    Json["Ambient"] = Object.Ambient;
    Json["Diffuse"] = Object.Diffuse;
    Json["Specular"] = Object.Specular;

}
