#include "Shader.h"

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <fstream>

#include "Misc/Logging.h"
#include "Renderer.h"

#if defined(COZ_EDITOR)
#include "Editor/DebugFrameBufferSubsystem.h"
#endif

void LShader::Load()
{
	std::ifstream VertexFile;
	std::ifstream FragmentFile;

	VertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	FragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	VertexFile.open(VertexShaderPath);
	FragmentFile.open(FragmentShaderPath);

	if (!VertexFile.is_open())
	{
		Log(LLogLevel::ERROR, "LShader::Load Failed to open " + VertexShaderPath);
		return;
	}

	if (!FragmentFile.is_open())
	{
		Log(LLogLevel::ERROR, "LShader::Load Failed to open " + FragmentShaderPath);
		return;
	}

	std::stringstream VertexStream;
	std::stringstream FragmentStream;

	VertexStream << VertexFile.rdbuf();
	FragmentStream << FragmentFile.rdbuf();

	std::string VertexCode = VertexStream.str();
	std::string FragmentCode = FragmentStream.str();

	const char* cVertexCode = VertexCode.c_str();
	const char* cFragmentCode = FragmentCode.c_str();

	VertexFile.close();
	FragmentFile.close();

	unsigned int VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderID, 1, &cVertexCode, NULL);
	glCompileShader(VertexShaderID);

	int  Success;
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Success);
#if defined(_DEBUG)
	char InfoLog[512];
	if (!Success)
	{
		glGetShaderInfoLog(VertexShaderID, 512, NULL, InfoLog);
		Log(LLogLevel::ERROR, "SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(InfoLog));
	}
#endif

	unsigned int FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderID, 1, &cFragmentCode, NULL);
	glCompileShader(FragmentShaderID);
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Success);
#if defined(_DEBUG)
	if (!Success)
	{
		glGetShaderInfoLog(FragmentShaderID, 512, NULL, InfoLog);
		Log(LLogLevel::ERROR, "SHADER::FRAGMENT::COMPILATION_FAILED\n" + std::string(InfoLog));
	}
#endif

	ID = glCreateProgram();
	glAttachShader(ID, VertexShaderID);
	glAttachShader(ID, FragmentShaderID);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &Success);
#if defined(_DEBUG)
	if (!Success)
	{
		glGetProgramInfoLog(ID, 512, NULL, InfoLog);
		Log(LLogLevel::ERROR, "SHADER::PROGRAM::COMPILATION_FAILED\n" + std::string(InfoLog));
	}
#endif

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	LRenderer* Renderer = CSystem.GetSubsystems().GetSubsystem<LRenderer>();
	if (bUsesMatricesUBO)
	{
		Renderer->RegisterMatricesUBOToShader(ID);
	}

	if (bUsesLightingUBO)
	{
		Renderer->ReigsterLightingUBOToShader(ID);
	}
}

void LShader::Unload()
{
	glDeleteProgram(ID);
}

bool LShader::HasRelevantShader() const
{
#if defined(COZ_EDITOR)
	LDebugFrameBufferSubsystem* DebugFrameBufferSubsystem = CSystem.GetSubsystems().GetSubsystem<LDebugFrameBufferSubsystem>();
	const std::string& ActiveFrameBuffer = DebugFrameBufferSubsystem->GetActiveDebugFrameBuffer();

	if (ActiveFrameBuffer.empty())
	{
		return true;
	}

	return DebugShaders.contains(ActiveFrameBuffer);
#else
	return true;
#endif
}

void LShader::Use()
{
	glUseProgram(GetActiveShaderID());
}

void LShader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(GetActiveShaderID(), name.c_str()), (int)value);
}

void LShader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(GetActiveShaderID(), name.c_str()), value);
}

void LShader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(GetActiveShaderID(), name.c_str()), value);
}

void LShader::SetVec3(const std::string& name, const glm::vec3& vec) const
{
	glUniform3fv(glGetUniformLocation(GetActiveShaderID(), name.c_str()), 1, &vec[0]);
}

void LShader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(GetActiveShaderID(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void LShader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(GetActiveShaderID(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

inline unsigned int LShader::GetActiveShaderID() const
{
#if defined(COZ_EDITOR)
	LDebugFrameBufferSubsystem* DebugFrameBufferSubsystem = CSystem.GetSubsystems().GetSubsystem<LDebugFrameBufferSubsystem>();

	const std::string& ActiveFrameBuffer = DebugFrameBufferSubsystem->GetActiveDebugFrameBuffer();

	if (ActiveFrameBuffer.empty() || !DebugShaders.contains(ActiveFrameBuffer))
	{
		return ID;
	}
	else
	{
		return DebugShaders.at(ActiveFrameBuffer).Get()->GetID();
	}
#else
	return ID;
#endif
}

void to_json(nlohmann::json& J, const LShader& Shader)
{
	J = nlohmann::json{
		{ "VertexShaderPath", Shader.VertexShaderPath },
		{ "FragmentShaderPath", Shader.FragmentShaderPath },
		{ "bUsesMatricesUBO", Shader.bUsesMatricesUBO },
		{ "bUsesLightingUBO", Shader.bUsesLightingUBO }
	};
	
#if defined(COZ_EDITOR)
	J["DebugShaders"] = Shader.DebugShaders;
#endif
}

void from_json(const nlohmann::json& J, LShader& Shader)
{
	Shader.VertexShaderPath = J.at("VertexShaderPath").get<std::string>();
	Shader.FragmentShaderPath = J.at("FragmentShaderPath").get<std::string>();
	Shader.bUsesMatricesUBO = J.at("bUsesMatricesUBO").get<bool>();
	Shader.bUsesLightingUBO = J.at("bUsesLightingUBO").get<bool>();

#if defined(COZ_EDITOR)
	Shader.DebugShaders = J["DebugShaders"];
#endif
}
