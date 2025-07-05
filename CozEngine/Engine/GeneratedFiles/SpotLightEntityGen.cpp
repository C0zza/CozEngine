#include "SpotLightEntity.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* CSpotLightEntity::Class = CSpotLightEntity::StaticClass();

LClass* CSpotLightEntity::StaticClass()
{
    if(!CSpotLightEntity::Class)
    {
        CSpotLightEntity::Class = new LClass();
        assert(CSpotLightEntity::Class);

        std::vector<LProperty> Properties
        {

        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {

            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new CSpotLightEntity();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) CSpotLightEntity();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<CSpotLightEntity*>(Add)->~CSpotLightEntity();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const CSpotLightEntity* Object = reinterpret_cast<const CSpotLightEntity*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                CSpotLightEntity* Object = reinterpret_cast<CSpotLightEntity*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(CSpotLightEntity::Class,
                                                        Properties,
                                                        sizeof(CSpotLightEntity),
                                                        alignof(CSpotLightEntity),
                                                        "CSpotLightEntity",
                                                        "LEntity",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("CSpotLightEntity", CSpotLightEntity::Class);
    }
    return CSpotLightEntity::Class;
}

void from_json(const nlohmann::json& Json, CSpotLightEntity& Object)
{
    LEntity& Parent = Object;
    from_json(Json, Parent);

}

void to_json(nlohmann::json& Json, const CSpotLightEntity& Object)
{
    const LEntity& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "CSpotLightEntity";

}
