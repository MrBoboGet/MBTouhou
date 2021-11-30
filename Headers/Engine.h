#pragma once
#include <Gameobject.h>
#include <Shader.h>
#include <Texture.h>
#include <string>
#include <gl3w/include/GL/gl3w.h>
#include <glfw/include/GLFW/glfw3.h>
#include <TouhouSoundEngine.h>
#include <MBUtility/MBMath.h>
#include <memory>
#include <MBGraphicsEngine/MBGE.h>
class ActiveObjectIterator
{
private:
	friend class TouhouEngine;
	bool m_HasEnded = false;
	std::vector<GameObject*>::iterator m_InternalIterator;
	std::vector<GameObject*>::iterator m_EndIterator;
public:
	bool HasEnded()
	{
		return(m_HasEnded);
	}

	bool operator==(ActiveObjectIterator const& OtherIterator)
	{
		return(m_HasEnded == OtherIterator.m_HasEnded);//
	}

	ActiveObjectIterator& operator++()
	{
		return(Increment());
	}
	ActiveObjectIterator& operator++(int)
	{
		return(Increment());
	}

	GameObject* operator*()
	{
		return(*m_InternalIterator);
	}
	GameObject* operator->()
	{
		return(*m_InternalIterator);
	}

	ActiveObjectIterator& Increment()
	{
		m_InternalIterator++;
		if (m_InternalIterator == m_EndIterator)
		{
			m_HasEnded = true;
		}
		return(*this);
	}
};
typedef MBGE::ShaderProgram Shader;
//struct GLFWwindow;
//struct GLFWmonitor;
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
	static GLFWwindow* CurrentWindow;

	static std::vector<GameObject*> DeletedGameObjects;
	static std::vector<GameObject*> ActiveGameObjects;
	static std::unordered_map<std::string, std::shared_ptr<Shader>> LoadedShaders;
	static std::unordered_map<std::string, std::shared_ptr<Texture>> LoadedTextures;

	static std::vector<DrawObject> DrawCalls;
	static MBGE::Camera __Camera;
	static std::unique_ptr<MBGE::SpriteModel> __SpriteModel;

	//static MBGE::MBGraphicsEngine __GraphicsEngine;

	static void p_DrawDrawObject(DrawObject& ObjectToDraw);
public:



	static float Framerate;
	TouhouEngine();
	~TouhouEngine();

	static GameObject* Create(GameObject* a);
	static void Destroy(GameObject* a);
	static GameObject* FindObjectWithName(std::string Name);
	static GameObject* FindObjectWithTag(std::string const& ObjectTag);
	static void Mainloop();
	static void InitializeWindow(int Width, int Height,std::string WindowName,GLFWmonitor* Monitor, GLFWwindow* Share);
	
	static bool GetKeyDown(std::string Key);
	static bool GetKeyPressed(std::string Key);
	static bool GetKeyReleased(std::string Key);


	static void _GetWindowSize(int* Width,int* Height);
	static ActiveObjectIterator GetActiveObjectsIterator();
	static void ClearObjects();
	static std::shared_ptr<Shader> GetNamedShader(std::string const& ShaderName);
	static std::shared_ptr<Texture> GetNamedTexture(std::string const& TextureName);
	static std::shared_ptr<Texture> LoadNamedTexture(std::string const& TextureName, std::string const& ResourcePath);
	static bool NamedTextureLoaded(std::string const& TextureName);
	
	static void DrawTexture(std::string NamedTexture, Vector2D Position, float Width, float Height, int Layer[]);
	static void DrawTexture(std::shared_ptr<Texture> TextureToDraw, Vector2D Position, float Width, float Height, int Layer[]);

	static void DebugLog(std::string TextToLog);

	static std::shared_ptr<Shader> LoadShader(std::string ShaderName,std::string VertexFilepath,std::string FragmentFilepath);
	
	static void PlaySound(std::string Path,float Volume);
	static void PlaySound(std::string, float RelativeVolume, std::string Tag);
private:

};