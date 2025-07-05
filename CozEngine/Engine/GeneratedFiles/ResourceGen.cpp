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

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) LResource();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<LResource*>(Add)->~LResource();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LResource* Object = reinterpret_cast<const LResource*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LResource* Object = reinterpret_cast<LResource*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LResource::Class,
                                                        Properties,
                                                        sizeof(LResource),
                                                        alignof(LResource),
                                                        "LResource",
                                                        "",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
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
