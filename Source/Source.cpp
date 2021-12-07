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
#include <MBGameEngine/MBGameEngine.h>

int main(void)
{
	//int qwerty = glfwInit();
	//std::filesystem::current_path("C:/Users/emanu/Desktop/Program/C++/MBTouhou/");
	MBGameEngine::MBGameEngine TestEngine;
	TestEngine.WindowCreate(1920, 1080, "OMyGodNej", false);
	std::cout << "Ny variant, garanterar recompile hoppas jag\n";
	//TouhouEngine::Create(new Level1()); asd
	TestEngine.Create(new MainMenu());
	std::shared_ptr<Shader> Shadern = TestEngine.LoadShader("SpriteShader", "Resources/Shaders/vertexshader.txt", "Resources/Shaders/fragmentshader.txt");
	std::shared_ptr<Shader> MonochromeShader = TestEngine.LoadShader("MonochromeShader", "Resources/Shaders/MonochromeShaderVertex.txt", "Resources/Shaders/MonochromeShaderFragment.txt");
	Shadern->Bind();
	Shadern->SetUniform1i("u_Texture", 0);
	Shadern->SetUniform4f("ColorKoef", 1, 1, 1, 1);

	TestEngine.MainLoop();
	glfwTerminate();
	return 0;
}