#include "PlayerEntity.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* CPlayerEntity::Class = CPlayerEntity::StaticClass();

LClass* CPlayerEntity::StaticClass()
{
    if(!CPlayerEntity::Class)
    {
        CPlayerEntity::Class = new LClass();
        assert(CPlayerEntity::Class);

        std::vector<LProperty> Properties
        {

        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new CPlayerEntity();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const CPlayerEntity* Object = reinterpret_cast<const CPlayerEntity*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                CPlayerEntity* Object = reinterpret_cast<CPlayerEntity*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(CPlayerEntity::Class,
                                                        Properties,
                                                        sizeof(CPlayerEntity),
                                                        alignof(CPlayerEntity),
                                                        "CPlayerEntity",
                                                        "LEntity",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc);
        
        LClassRegister::RegisterObj("CPlayerEntity", CPlayerEntity::Class);
    }
    return CPlayerEntity::Class;
}

void from_json(const nlohmann::json& Json, CPlayerEntity& Object)
{
    LEntity& Parent = Object;
    from_json(Json, Parent);

}

void to_json(nlohmann::json& Json, const CPlayerEntity& Object)
{
    const LEntity& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "CPlayerEntity";

}
