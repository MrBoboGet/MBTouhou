//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include<iostream>
#include <fstream>
#include <string>
#include <ctime>
//#include <gl3w/include/GL/gl3w.h>
#include <vector>
#include <Gameobject.h>
#include <SpelObjekt.h>
#include <unordered_map>
#include <Hitbox.h>
#include <Engine.h>
#include <MainMenu.h>
#include <filesystem>
#include <memory>
//vet itne varför denna behövs
//TODO Ha johan som boss för banan
//pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
int main(void)
{
	int qwerty = glfwInit();
	std::filesystem::current_path("C:/Users/emanu/Desktop/Program/C++/MBTouhou/");
	TouhouEngine::InitializeWindow(1920, 1080, "OMyGodNej", glfwGetPrimaryMonitor(), NULL);
	//TouhouEngine::Create(new Level1()); asd
	TouhouEngine::Create(new MainMenu());
	std::shared_ptr<Shader> Shadern = TouhouEngine::LoadShader("SpriteShader", "vertexshader.txt", "fragmentshader.txt");
	std::shared_ptr<Shader> MonochromeShader = TouhouEngine::LoadShader("MonochromeShader", "MonochromeShaderVertex.txt", "MonochromeShaderFragment.txt");
	Shadern->Bind();
	Shadern->SetUniform1i("u_Texture", 0);
	Shadern->SetUniform4f("ColorKoef", 1, 1, 1, 1);

	TouhouEngine::Mainloop();
	glfwTerminate();
	return 0;
}