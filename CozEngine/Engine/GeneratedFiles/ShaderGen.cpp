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

        LClassUtilities::RegisterStaticClassProperties(LShader::Class,
                                                        Properties,
                                                        sizeof(LShader),
                                                        alignof(LShader),
                                                        "LShader",
                                                        "LResource",
                                                        DrawEditorFunc,
                                                        CreateObjectFunc);
        
        LClassRegister::RegisterObj("LShader", LShader::Class);
    }
    return LShader::Class;
}

void from_json(const nlohmann::json& Json, LShader& Object)
{
    LResource& Parent = Object;
    from_json(Json, Parent);
    Object.VertexShaderPath = Json["VertexShaderPath"];
    Object.FragmentShaderPath = Json["FragmentShaderPath"];
    Object.bUsesMatricesUBO = Json["bUsesMatricesUBO"];
    Object.bUsesLightingUBO = Json["bUsesLightingUBO"];

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
