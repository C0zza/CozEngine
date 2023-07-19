#pragma once

#include <string>

class Shader
{
public:
	Shader(const char* VertexPath, const char* FragmentPath);

	unsigned int GetID() { return ID; }

	void Use();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
private:
	
	unsigned int ID;
};
