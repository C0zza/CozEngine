#include "Rendering/Model.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LModel::Class = LModel::StaticClass();

LClass* LModel::StaticClass()
{
    if(!LModel::Class)
    {
        LModel::Class = new LClass();
        assert(LModel::Class);

        std::vector<LProperty> Properties
        {
            {"ObjFile", "std::string", MEMBER_OFFSET(LModel, ObjFile)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                std::string* ObjFilePtr = reinterpret_cast<std::string*>(Address + offsetof(LModel, ObjFile));
                LImGuiPropertyDrawHelpers::DrawProperty("ObjFile", *ObjFilePtr);
            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LModel();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) LModel();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<LModel*>(Add)->~LModel();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LModel* Object = reinterpret_cast<const LModel*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LModel* Object = reinterpret_cast<LModel*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LModel::Class,
                                                        Properties,
                                                        sizeof(LModel),
                                                        alignof(LModel),
                                                        "LModel",
                                                        "LResource",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LModel", LModel::Class);
    }
    return LModel::Class;
}

void from_json(const nlohmann::json& Json, LModel& Object)
{
    LResource& Parent = Object;
    from_json(Json, Parent);
    if(Json.contains("ObjFile")) Object.ObjFile = Json["ObjFile"];

}

void to_json(nlohmann::json& Json, const LModel& Object)
{
    const LResource& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LModel";
    Json["ObjFile"] = Object.ObjFile;

}
