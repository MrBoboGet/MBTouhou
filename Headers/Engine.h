#pragma once
#include <Gameobject.h>
#include <Shader.h>
#include <Texture.h>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <TouhouSoundEngine.h>
#include <MBUtility/MBMath.h>

class TouhouEngine
{
	friend class TouhouSoundEngineSoundObject;
private:
	static void DeleteObjectsOutsideScreen();
	static void HandleDeletedGameobjects();
	static void UpdateGameobjects(std::vector<GameObject*> GameobjectsToUpdate);
	static void Render(std::vector<GameObject*> ObjectsToRender);
	static bool UpdatePause;
	static void UpdateKeys();
	static std::vector<std::string> KeyList;
	static std::unordered_map<std::string, bool> PressedKeys;
	static std::unordered_map<std::string, bool> PreviouslyPressedKeys;
	static void InitializeEngineCompononets();

	//static std::vector<void(*)()> CustomUpdateProcedures;
	//static void RunCostumUpdateProcedures();
	static void UpdateSoundSystem();
	static void CleanupFinishedSounds();
	static TouhouSoundEngine SoundEngine;
	static std::vector<TouhouSoundEngineSoundObject*> SoundObjects;
public:
	static GLFWwindow* CurrentWindow;
	static std::unordered_map<std::string, Shader*> LoadedShaders;
	static std::unordered_map<std::string, Texture*> LoadedTextures;

	static std::vector<GameObject*> ActiveGameObjects;
	static std::vector<GameObject*> DeletedGameObjects;
	static std::vector<DrawObject*> DrawCalls;

	static float Framerate;
	TouhouEngine();
	~TouhouEngine();

	static GameObject* Create(GameObject* a);
	static void Destroy(GameObject* a);
	static GameObject* FindObjectWithName(std::string Name);
	static void Mainloop();
	static void InitializeWindow(int Width, int Height,std::string WindowName,GLFWmonitor* Monitor, GLFWwindow* Share);
	
	static bool GetKeyDown(std::string Key);
	static bool GetKeyPressed(std::string Key);
	static bool GetKeyReleased(std::string Key);

	static void DebugLog(std::string TextToLog);

	static Shader* LoadShader(std::string ShaderName,std::string VertexFilepath,std::string FragmentFilepath);
	
	static void PlaySound(std::string Path,float Volume);
	static void PlaySound(std::string, float RelativeVolume, std::string Tag);
private:

};