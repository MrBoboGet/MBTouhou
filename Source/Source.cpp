#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <Renderer.h> 
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <VertexBufferLayout.h>
#include <Texture.h>
#include <vector>
#include <Gameobject.h>
#include <SpelObjekt.h>
#include <MinMath.h>
#include <unordered_map>
#include <Hitbox.h>
#include <Engine.h>
#include<MainMenu.h>
#include <fmod.hpp>
#include <filesystem>
//vet itne varför denna behövs
//TODO Ha johan som boss för banan
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
int qwerty = glfwInit();
int main(void)
{

	std::filesystem::current_path("C:/Users/emanu/Desktop/Program/C++/MBTouhou/");
	TouhouEngine::InitializeWindow(1920, 1080, "OMyGodNej", glfwGetPrimaryMonitor(), NULL);
	//TouhouEngine::Create(new Level1());
	TouhouEngine::Create(new MainMenu());
	Shader* Shadern = TouhouEngine::LoadShader("SpriteShader","vertexshader.txt", "fragmentshader.txt");
	Shader* MonochromeShader = TouhouEngine::LoadShader("MonochromeShader", "MonochromeShaderVertex.txt", "MonochromeShaderFragment.txt");
	Shadern->Bind();
	Shadern->SetUniform1i("u_Texture", 0);

	TouhouEngine::Mainloop();
	glfwTerminate();
	return 0;
}