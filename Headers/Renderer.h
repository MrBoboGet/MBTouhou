#pragma once
//#include <Gl/glew.h>
#include <gl3w/include/GL/gl3w.h>
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__,__LINE__))
#include <VertexArray.h>
#include <IndexBuffer.h>
#include <Shader.h>
void GLClearError();
void GLCheckError();
bool GLLogCall(const char* function, const char* file, int line);
//class Renderer
//{
//
//public:
//	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
//	void Clear() const;
//};
