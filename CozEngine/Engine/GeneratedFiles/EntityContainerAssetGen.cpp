#include "ECS/EntityManagement/EntityContainerAsset.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LEntityContainerAsset::Class = LEntityContainerAsset::StaticClass();

LClass* LEntityContainerAsset::StaticClass()
{
    if(!LEntityContainerAsset::Class)
    {
        LEntityContainerAsset::Class = new LClass();
        assert(LEntityContainerAsset::Class);

        std::vector<LProperty> Properties
        {
            {"EntitiesData", "nlohmann::json", MEMBER_OFFSET(LEntityContainerAsset, EntitiesData)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LEntityContainerAsset();
            };

        LClassUtilities::RegisterStaticClassProperties(LEntityContainerAsset::Class,
                                                        Properties,
                                                        sizeof(LEntityContainerAsset),
                                                        alignof(LEntityContainerAsset),
                                                        "LEntityContainerAsset",
                                                        "LResource",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc);
        
        LClassRegister::RegisterObj("LEntityContainerAsset", LEntityContainerAsset::Class);
    }
    return LEntityContainerAsset::Class;
}

void from_json(const nlohmann::json& Json, LEntityContainerAsset& Object)
{
    LResource& Parent = Object;
    from_json(Json, Parent);
    Object.EntitiesData = Json["EntitiesData"];

}

void to_json(nlohmann::json& Json, const LEntityContainerAsset& Object)
{
    const LResource& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LEntityContainerAsset";
    Json["EntitiesData"] = Object.EntitiesData;

}
