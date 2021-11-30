#include <Shader.h>
#include <Renderer.h>
#include <GL/glew.h>
#include <iostream>
#include <fstream>

//Shader::Shader(const std::string& vertexshader_filepath,const std::string& fragmentshader_filepath)
//	: m_FsFilePath(fragmentshader_filepath), m_RendererID(0),m_VsFilepath(vertexshader_filepath)
//{
//	std::ifstream Vs_Source(m_VsFilepath);
//	std::ifstream Fs_Source(m_FsFilePath);
//	std::string Vs = "";
//	std::string Fs = "";
//	for (std::string line; std::getline(Vs_Source, line);)
//	{
//		Vs += line +"\n";
//	}
//	for (std::string line; std::getline(Fs_Source, line);)
//	{
//		Fs += line + "\n";
//	}
//	m_RendererID = CreateShader(Vs,Fs);
//}
//bool Shader::CompileShader()
//{
//	return(true);
//}
//Shader::~Shader()
//{
//	std::cout << " shader kanske inte heller borde shallow kopieras" << std::endl;
//	GLCall(glDeleteProgram(m_RendererID));
//}
//void Shader::Bind() const
//{
//	GLCall(glUseProgram(m_RendererID));
//}
//void Shader::UnBind() const
//{
//	std::cout <<"shadern blev tydligen icke bindad" << std::endl;
//	GLCall(glUseProgram(0));
//}
//void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
//{
//	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
//}
//int Shader::GetUniformLocation(const std::string& name)
//{
//	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
//	{
//		return m_UniformLocationCache[name];
//	}
//	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
//	if (location == -1)
//	{
//
//	}
//	m_UniformLocationCache[name] = location;
//	return location;
//}
//void Shader::SetUniform1i(const std::string& name, int value)
//{
//	GLCall(glUniform1i(GetUniformLocation(name), value));
//}
//void Shader::SetUniform1f(const std::string& name, float value)
//{
//
//	GLCall(glUniform1f(GetUniformLocation(name), value));
//}
//unsigned int Shader::Compileshader(unsigned int type, const std::string& source)
//{
//	unsigned int id = glCreateShader(type);
//	const char* src = source.c_str();
//	glShaderSource(id, 1, &src, nullptr);
//	glCompileShader(id);
//	int result;
//	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
//	if (result == GL_FALSE)
//	{
//		int length;
//		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
//		char* message = (char*)alloca(length * sizeof(char));
//		glGetShaderInfoLog(id, length, &length, message);
//		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
//		std::cout << message << std::endl;
//		glDeleteShader(id);
//		return (0);
//	}
//	return(id);
//}
//int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
//{
//	unsigned int program = glCreateProgram();
//	unsigned int vs = Compileshader(GL_VERTEX_SHADER, vertexShader);
//	unsigned int fs = Compileshader(GL_FRAGMENT_SHADER, fragmentShader);
//	glAttachShader(program, vs);
//	glAttachShader(program, fs);
//	glLinkProgram(program);
//	glValidateProgram(program);
//
//	return(program);
//
//}
//Shader::Shader()
//{
//
//}