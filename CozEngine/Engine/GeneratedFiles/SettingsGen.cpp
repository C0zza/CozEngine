#include "Settings/Settings.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LSettings::Class = LSettings::StaticClass();

LClass* LSettings::StaticClass()
{
    if(!LSettings::Class)
    {
        LSettings::Class = new LClass();
        assert(LSettings::Class);

        std::vector<LProperty> Properties
        {

        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LSettings();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) LSettings();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<LSettings*>(Add)->~LSettings();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LSettings* Object = reinterpret_cast<const LSettings*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LSettings* Object = reinterpret_cast<LSettings*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LSettings::Class,
                                                        Properties,
                                                        sizeof(LSettings),
                                                        alignof(LSettings),
                                                        "LSettings",
                                                        "",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LSettings", LSettings::Class);
    }
    return LSettings::Class;
}

void from_json(const nlohmann::json& Json, LSettings& Object)
{


}

void to_json(nlohmann::json& Json, const LSettings& Object)
{

    Json["Type"] = "LSettings";

}
