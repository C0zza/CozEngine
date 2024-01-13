#include "Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<LShader*> LShader::Shaders = {};

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
		std::cout << "Shader::Shader - Failed to open " << VertexShaderPath << ".\n";
		return;
	}

	if (!FragmentFile.is_open())
	{
		std::cout << "Shader::Shader - Failed to open " << FragmentShaderPath << ".\n";
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
	char InfoLog[512];
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(VertexShaderID, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << InfoLog << std::endl;
	}

	unsigned int FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderID, 1, &cFragmentCode, NULL);
	glCompileShader(FragmentShaderID);
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(FragmentShaderID, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << InfoLog << "\n";

	}

	ID = glCreateProgram();
	glAttachShader(ID, VertexShaderID);
	glAttachShader(ID, FragmentShaderID);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(ID, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << InfoLog << "\n";
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	Shaders.push_back(this);
}

void LShader::Unload()
{
	unsigned int Index = -1;
	for (unsigned int i = 0; i < Shaders.size(); i++)
	{
		if (Shaders[i] == this)
		{
			Index = i;
		}
	}

	if (Index >= 0)
	{
		Shaders.erase(Shaders.begin() + Index);
	}
}

void LShader::Use()
{
	glUseProgram(ID);
}

void LShader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void LShader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void LShader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void LShader::SetVec3(const std::string& name, const glm::vec3& vec) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}

void LShader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void LShader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void LShader::SetGlobalBool(const std::string& name, const bool b)
{
	for (LShader* Shader : Shaders)
	{
		assert(Shader);
		Shader->Use();
		Shader->SetBool(name, b);
	}
}

void LShader::SetGlobalInt(const std::string& name, const int i)
{
	for (LShader* Shader : Shaders)
	{
		assert(Shader);
		Shader->Use();
		Shader->SetInt(name, i);
	}
}

void LShader::SetGlobalFloat(const std::string& name, const float f)
{
	for (LShader* Shader : Shaders)
	{
		assert(Shader);
		Shader->Use();
		Shader->SetFloat(name, f);
	}
}

void LShader::SetGlobalMat4(const std::string& name, const glm::mat4& mat)
{
	for (LShader* Shader : Shaders)
	{
		assert(Shader);
		Shader->Use();
		Shader->SetMat4(name, mat);
	}
}

void LShader::SetGlobalVec(const std::string& name, const glm::vec3& vec)
{
	for (LShader* Shader : Shaders)
	{
		assert(Shader);
		Shader->Use();
		Shader->SetVec3(name, vec);
	}
}
