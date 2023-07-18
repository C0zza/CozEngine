#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

void CheckShaderCompilation(const unsigned int ID)
{
	int Success;
	char InfoLog[512];
	glGetShaderiv(ID, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(ID, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << InfoLog << "\n";
	}
}

void CheckProgramCompilation(const unsigned int ID)
{
	int Success;
	char InfoLog[512];
	glGetProgramiv(ID, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(ID, 512, NULL, InfoLog);
		std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << InfoLog << "\n";
	}
}

Shader::Shader(const char* VertexPath, const char* FragmentPath)
{
	std::ifstream VertexFile;
	std::ifstream FragmentFile;

	VertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	FragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	VertexFile.open(VertexPath);
	FragmentFile.open(FragmentPath);

	if (!VertexFile.is_open())
	{
		std::cout << "Shader::Shader - Failed to open " << VertexPath << ".\n";
		return;
	}

	if (!FragmentFile.is_open())
	{
		std::cout << "Shader::Shader - Failed to open " << FragmentPath << ".\n";
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
	CheckShaderCompilation(VertexShaderID);

	unsigned int FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderID, 1, &cFragmentCode, NULL);
	glCompileShader(FragmentShaderID);
	CheckShaderCompilation(FragmentShaderID);

	ID = glCreateProgram();
	glAttachShader(ID, VertexShaderID);
	glAttachShader(ID, FragmentShaderID);
	glLinkProgram(ID);
	CheckProgramCompilation(ID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
