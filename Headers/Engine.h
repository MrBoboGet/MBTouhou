#pragma once
#include <Gameobject.h>
#include <string>
#include <gl3w/include/GL/gl3w.h>
#include <glfw/include/GLFW/glfw3.h>
#include <TouhouSoundEngine.h>
#include <MBUtility/MBMath.h>
#include <memory>
#include <MBGraphicsEngine/MBGE.h>
#include <array>
#include <MBGameEngine/MBGameEngine.h>

typedef MBGE::ShaderProgram Shader;
typedef MBGE::Texture Texture;
struct ColorRGBA
{
	float R = 1;
	float G = 1;
	float B = 1;
	float A = 1;
};

typedef MBGE::Texture Texture;
class DrawObject
{
public:
	std::shared_ptr<Texture> Texturen = nullptr;
	ColorRGBA Color;
	Vector2D Position;
	float Width;
	float Height;
	std::vector<int> Layer = { 0,0,0,0 };

	//DrawObject(std::string a, Vector2D b, float c, float d, int Layern[]);
	//DrawObject(Vector2D Start,Vector2D End ,std::vector<int> Layern);
};
//iomed att jag är lite lat och vet att vi inte igentligen behöver ändra shadern så kommer jag från och med nu antat att varje objekt inte behöver en unik
class GameObjectRenderer
{
public:
	struct ColorRGBA
	{
		float R = 1;
		float G = 1;
		float B = 1;
		float A = 1;
	};
	ColorRGBA ColorKoef;
	float Width = 1;
	std::vector<int> Layer = { 0,0,0,0 };
	//ett problem vi får med den här koden är att varje gameobject gör sin egen kopia av samma texture, vilket tar minne och eventuellt prestanda
	//tanken är att jag gör en dictionary så att om en texture redans finns så säger vi att render idn är den texturen
	std::shared_ptr<Texture> SpriteTexture = nullptr;
	//Shader ObjectShader;
	//GameObjectRenderer(std::string Bild);
	//GameObjectRenderer(std::string Namn, float Storlek);
	GameObjectRenderer();
	~GameObjectRenderer();
};
class GameObject;
class GameObject;
class Component
{
protected:
	GameObject* SpelObjectet;
public:
	//std::string ComponentName;
	//virtual std::vector<std::string> RequiredResources();
	GameObject* GetGameObject()
	{
		return(SpelObjectet);
	}
	const GameObject* GetGameObject() const
	{
		return(SpelObjectet);
	}
	Component();
	~Component();
	virtual void Update();
private:

};

class GameObject
{
private:
	friend class TouhouEngine;
	std::string Name;
	std::string Tag;
	std::vector<Component*> Components = std::vector<Component*>(0);
	std::unordered_map<std::string, Component*> ComponentDictionary = std::unordered_map<std::string, Component*>(0);
	void UpdateComponents();
public:
	Vector2D Position;
	float Rotation = 0;
	MBGameEngine::Transform Transform;

	int HP;
	GameObjectRenderer Renderer;


	bool Active = true;
	//Vector2D Hitbox;
	//komponent grejer
	void AddComponent(Component* ComponentName); //ok inteface, kan stanna kvar
	//Component* GetComponent(std::string ComponentName);
	template<typename T> T* GetComponent()
	{
		T* ReturnValue = nullptr;
		for (size_t i = 0; i < Components.size(); i++)
		{
			ReturnValue = dynamic_cast<T*>(Components[i]);
			if (ReturnValue != nullptr)
			{
				break;
			}
		}
		return(ReturnValue);
	}
	virtual void Update();
	virtual void OnCreate();
	//virtual void Collision();
	void Render();
	GameObject();
	GameObject(Vector2D Plats, std::string Namn, std::string Tagg);
	GameObject(std::string Namn, float Storlek);
	virtual ~GameObject();
	inline std::string GetTag()
	{
		return(Tag);
	}
	inline std::string GetName()
	{
		return(Name);
	}
	void SetName(std::string NameToSet)
	{
		Name = std::move(NameToSet);
	}
	void SetTag(std::string TagToSet)
	{
		Tag = std::move(TagToSet);
	}
	//någon form av rendering funktion, eller är det något vi kan lägga till efter? 
	//man skulle kunna göra update och liknande virtual, och ha grundläggande grejer i dem så att man alltid vet att dem har en
};
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
	
	static void DrawTexture(std::string const& NamedTexture, Vector2D Position, float Width, float Height, std::array<int,4> Layer);
	static void DrawTexture(std::shared_ptr<Texture> TextureToDraw, Vector2D Position, float Width, float Height, std::array<int, 4> Layer);

	static void DebugLog(std::string TextToLog);

	static std::shared_ptr<Shader> LoadShader(std::string ShaderName,std::string VertexFilepath,std::string FragmentFilepath);
	
	static void PlaySound(std::string Path,float Volume);
	static void PlaySound(std::string, float RelativeVolume, std::string Tag);
private:

};