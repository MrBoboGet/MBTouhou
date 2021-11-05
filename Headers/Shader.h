#pragma once

#include <string>
#include <unordered_map>
class Shader
{
private:
	std::string m_FsFilePath;
	std::string m_VsFilepath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
	// caching for uniforms
public:
	Shader(const std::string& vertexshader_filepath, const std::string& fragmentshader_filepath);
	Shader();
	~Shader();

	void Bind() const;
	void UnBind() const;
	//att skapa shadern
	int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int Compileshader(unsigned int type, const std::string& source);
	//set uniform
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name,float v0,float v1, float v2, float v3);
	inline unsigned int GetID()
	{
		return(m_RendererID);
	}
private:
	int GetUniformLocation(const std::string& name);
	bool  CompileShader();
};