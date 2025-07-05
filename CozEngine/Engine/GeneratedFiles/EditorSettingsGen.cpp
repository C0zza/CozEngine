#include "Editor/EditorSettings.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"
#include "ResourceManagement/ResourceHandleHelper.h"


LClass* LEditorSettings::Class = LEditorSettings::StaticClass();

LClass* LEditorSettings::StaticClass()
{
    if(!LEditorSettings::Class)
    {
        LEditorSettings::Class = new LClass();
        assert(LEditorSettings::Class);

        std::vector<LProperty> Properties
        {
            {"EditorStartupLevel", "LSoftResourceHandle<LEntityContainerAsset>", MEMBER_OFFSET(LEditorSettings, EditorStartupLevel)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                LSoftResourceHandle<LEntityContainerAsset>* EditorStartupLevelPtr = reinterpret_cast<LSoftResourceHandle<LEntityContainerAsset>*>(Address + offsetof(LEditorSettings, EditorStartupLevel));
                LImGuiPropertyDrawHelpers::DrawProperty("EditorStartupLevel", *EditorStartupLevelPtr);
            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LEditorSettings();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) LEditorSettings();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<LEditorSettings*>(Add)->~LEditorSettings();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LEditorSettings* Object = reinterpret_cast<const LEditorSettings*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LEditorSettings* Object = reinterpret_cast<LEditorSettings*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LEditorSettings::Class,
                                                        Properties,
                                                        sizeof(LEditorSettings),
                                                        alignof(LEditorSettings),
                                                        "LEditorSettings",
                                                        "LSettings",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LEditorSettings", LEditorSettings::Class);
    }
    return LEditorSettings::Class;
}

void from_json(const nlohmann::json& Json, LEditorSettings& Object)
{
    LSettings& Parent = Object;
    from_json(Json, Parent);
    if(Json.contains("EditorStartupLevel")) Object.EditorStartupLevel = Json["EditorStartupLevel"];

}

void to_json(nlohmann::json& Json, const LEditorSettings& Object)
{
    const LSettings& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LEditorSettings";
    Json["EditorStartupLevel"] = Object.EditorStartupLevel;

}
