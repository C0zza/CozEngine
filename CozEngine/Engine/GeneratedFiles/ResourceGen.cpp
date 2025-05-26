#include "ResourceManagement/Resource.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LResource::Class = LResource::StaticClass();

LClass* LResource::StaticClass()
{
    if(!LResource::Class)
    {
        LResource::Class = new LClass();
        assert(LResource::Class);

        std::vector<LProperty> Properties
        {

        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LResource();
            };

        LClassUtilities::RegisterStaticClassProperties(LResource::Class,
                                                        Properties,
                                                        sizeof(LResource),
                                                        alignof(LResource),
                                                        "LResource",
                                                        "",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc);
        
        LClassRegister::RegisterObj("LResource", LResource::Class);
    }
    return LResource::Class;
}

void from_json(const nlohmann::json& Json, LResource& Object)
{


}

void to_json(nlohmann::json& Json, const LResource& Object)
{

    Json["Type"] = "LResource";

}
