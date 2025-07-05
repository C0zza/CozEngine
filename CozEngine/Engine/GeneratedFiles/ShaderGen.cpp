#include "Rendering/Shader.h"

#include "Editor/ImGuiPropertyDrawHelpers.h"
#include "Reflection/Class.h"
#include "Reflection/ClassRegister.h"
#include "Reflection/ClassUtilities.h"
#include "Reflection/Property.h"


LClass* LShader::Class = LShader::StaticClass();

LClass* LShader::StaticClass()
{
    if(!LShader::Class)
    {
        LShader::Class = new LClass();
        assert(LShader::Class);

        std::vector<LProperty> Properties
        {
            {"VertexShaderPath", "std::string", MEMBER_OFFSET(LShader, VertexShaderPath)},
            {"FragmentShaderPath", "std::string", MEMBER_OFFSET(LShader, FragmentShaderPath)},
            {"bUsesMatricesUBO", "bool", MEMBER_OFFSET(LShader, bUsesMatricesUBO)},
            {"bUsesLightingUBO", "bool", MEMBER_OFFSET(LShader, bUsesLightingUBO)}
        };

        std::function<void(uint8_t*)> DrawEditorFunc = [](uint8_t* Address)
            {
                std::string* VertexShaderPathPtr = reinterpret_cast<std::string*>(Address + offsetof(LShader, VertexShaderPath));
                LImGuiPropertyDrawHelpers::DrawProperty("VertexShaderPath", *VertexShaderPathPtr);

                std::string* FragmentShaderPathPtr = reinterpret_cast<std::string*>(Address + offsetof(LShader, FragmentShaderPath));
                LImGuiPropertyDrawHelpers::DrawProperty("FragmentShaderPath", *FragmentShaderPathPtr);

                bool* bUsesMatricesUBOPtr = reinterpret_cast<bool*>(Address + offsetof(LShader, bUsesMatricesUBO));
                LImGuiPropertyDrawHelpers::DrawProperty("bUsesMatricesUBO", *bUsesMatricesUBOPtr);

                bool* bUsesLightingUBOPtr = reinterpret_cast<bool*>(Address + offsetof(LShader, bUsesLightingUBO));
                LImGuiPropertyDrawHelpers::DrawProperty("bUsesLightingUBO", *bUsesLightingUBOPtr);
            };

        
        std::function<void*()> CreateObjectFunc = []()
            {
                return new LShader();
            };

        std::function<void(uint8_t*)> ConstructAddressFunc = [](uint8_t* Add)
            {
                new (Add) LShader();
            };

        std::function<void(uint8_t*)> DestructAddressFunc = [](uint8_t* Add)
            {
                reinterpret_cast<LShader*>(Add)->~LShader();
            };

        std::function<void(const uint8_t*, nlohmann::json& Json)> SerializeFunc = [](const uint8_t* Address, nlohmann::json& Json)
            {
                const LShader* Object = reinterpret_cast<const LShader*>(Address);
                to_json(Json, *Object);
            };

        std::function<void(uint8_t*, const nlohmann::json& Json)> DeserializeFunc = [](uint8_t* Address, const nlohmann::json& Json)
            {
                LShader* Object = reinterpret_cast<LShader*>(Address);
                from_json(Json, *Object);
            };

        LClassUtilities::RegisterStaticClassProperties(LShader::Class,
                                                        Properties,
                                                        sizeof(LShader),
                                                        alignof(LShader),
                                                        "LShader",
                                                        "LResource",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc,
                                                        SerializeFunc,
                                                        DeserializeFunc,
                                                        ConstructAddressFunc,
                                                        DestructAddressFunc);
        
        LClassRegister::RegisterObj("LShader", LShader::Class);
    }
    return LShader::Class;
}

void from_json(const nlohmann::json& Json, LShader& Object)
{
    LResource& Parent = Object;
    from_json(Json, Parent);
    if(Json.contains("VertexShaderPath")) Object.VertexShaderPath = Json["VertexShaderPath"];
    if(Json.contains("FragmentShaderPath")) Object.FragmentShaderPath = Json["FragmentShaderPath"];
    if(Json.contains("bUsesMatricesUBO")) Object.bUsesMatricesUBO = Json["bUsesMatricesUBO"];
    if(Json.contains("bUsesLightingUBO")) Object.bUsesLightingUBO = Json["bUsesLightingUBO"];

}

void to_json(nlohmann::json& Json, const LShader& Object)
{
    const LResource& Parent = Object;
    to_json(Json, Parent);
    Json["Type"] = "LShader";
    Json["VertexShaderPath"] = Object.VertexShaderPath;
    Json["FragmentShaderPath"] = Object.FragmentShaderPath;
    Json["bUsesMatricesUBO"] = Object.bUsesMatricesUBO;
    Json["bUsesLightingUBO"] = Object.bUsesLightingUBO;

}
