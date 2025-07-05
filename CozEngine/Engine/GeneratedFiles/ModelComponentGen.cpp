#include "ECS/ECSComponents/ModelComponent.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"
#include "ResourceManagement/ResourceHandleHelper.h"


LClass* CModelComponent::Class = CModelComponent::StaticClass();

LClass* CModelComponent::StaticClass()
{
    if(!CModelComponent::Class)
    {
        CModelComponent::Class = new LClass();
        assert(CModelComponent::Class);

        std::vector<LProperty> Properties
        {
            {"Model", "LResourceHandle<LModel>", MEMBER_OFFSET(CModelComponent, Model)},
            {"ModelMaterial", "LResourceHandle<LModelMaterial>", MEMBER_OFFSET(CModelComponent, ModelMaterial)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                LResourceHandle<LModel>* ModelPtr = reinterpret_cast<LResourceHandle<LModel>*>(Address + offsetof(CModelComponent, Model));
                LImGuiPropertyDrawHelpers::DrawProperty("Model", *ModelPtr);

                LResourceHandle<LModelMaterial>* ModelMaterialPtr = reinterpret_cast<LResourceHandle<LModelMaterial>*>(Address + offsetof(CModelComponent, ModelMaterial));
                LImGuiPropertyDrawHelpers::DrawProperty("ModelMaterial", *ModelMaterialPtr);
            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new CModelComponent();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) CModelComponent();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<CModelComponent*>(Add)->~CModelComponent();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const CModelComponent* Object = reinterpret_cast<const CModelComponent*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                CModelComponent* Object = reinterpret_cast<CModelComponent*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(CModelComponent::Class,
                                                        Properties,
                                                        sizeof(CModelComponent),
                                                        alignof(CModelComponent),
                                                        "CModelComponent",
                                                        "LECSComponent",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("CModelComponent", CModelComponent::Class);
    }
    return CModelComponent::Class;
}

void from_json(const nlohmann::json& Json, CModelComponent& Object)
{
    LECSComponent& Parent = Object;
    from_json(Json, Parent);
    if(Json.contains("Model")) Object.Model = Json["Model"];
    if(Json.contains("ModelMaterial")) Object.ModelMaterial = Json["ModelMaterial"];

}

void to_json(nlohmann::json& Json, const CModelComponent& Object)
{
    const LECSComponent& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "CModelComponent";
    Json["Model"] = Object.Model;
    Json["ModelMaterial"] = Object.ModelMaterial;

}
