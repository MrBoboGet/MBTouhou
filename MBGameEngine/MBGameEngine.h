#pragma once
#include <vector>
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <MBGraphicsEngine/MBGE.h>
#include <array>
namespace MBGameEngine
{
	enum class KeyCode
	{
		A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
		Shift,Left,Right,Up,Down,
	};

	//class Transform
	//{
	//
	//public:
	//	MBMath::MBVector3<float> Position;
	//};
	typedef MBGE::Transform Transform;
	typedef MBGE::Texture Texture;
	typedef MBGE::ShaderProgram Shader;
	template<typename T>
	class SharedDeletableObjectReference
	{
	private:
		std::shared_ptr<T*> m_ObjectPointer = nullptr;
		void p_Validate()
		{
			if (!(*this))
			{
				throw std::exception();
			}
		}
	public:
		SharedDeletableObjectReference(std::shared_ptr<T*> PointerToUse)
		{
			m_ObjectPointer = PointerToUse;
		}
		SharedDeletableObjectReference()
		{

		}
		operator bool()
		{
			return(m_ObjectPointer.get() != nullptr && *m_ObjectPointer != nullptr);
		}
		bool operator==(T* OtherPointer)
		{
			if (m_ObjectPointer.get() == nullptr)
			{
				return(false);
			}
			return(*m_ObjectPointer == OtherPointer);
		}
		T* operator->()
		{
			p_Validate();
			return *m_ObjectPointer;
		}
		T& operator*()
		{
			p_Validate();
			return **m_ObjectPointer;
		}
		T const* operator->() const
		{
			p_Validate();
			return *m_ObjectPointer;
		}
		T const& operator*() const
		{
			p_Validate();
			return **m_ObjectPointer;
		}
	};
	// template <typename T>
	// using SharedDeletableObjectReference<GameObject> = GameObjectReference;
	class GameObject;
	class Component;
	typedef SharedDeletableObjectReference<GameObject> GameObjectReference;
	typedef SharedDeletableObjectReference<Component> ComponentReference;
	template <typename T>
	using ObjectReference = SharedDeletableObjectReference<T>;

	class Component
	{
	private:

		GameObjectReference m_AssociatedGameObject;
		std::shared_ptr<Component*> m_ComponentReference = nullptr;

	public:
		GameObjectReference GetGameObject();
		GameObjectReference GetGameObject() const;
		virtual void Update();
		virtual ~Component();
	};

	class MBGameEngine;
	class GameObject
	{
	private:
		friend class MBGameEngine;
		friend class ActiveObjectIterator;
		std::string m_Name = "";
		std::string m_Tag = "";
		bool m_Active = true;


		std::shared_ptr<GameObject*> m_ObjectReferencePointer = {};
		MBGameEngine* m_AssociatedEngine = nullptr;
		std::vector<std::unique_ptr<Component>> m_Components = {};
		void p_DefaultUpdate();
		//GameObject(MBGameEngine* AssociatedEngine);
		bool m_Deleted = false;
		//bool IsDeleted();
	protected:
		MBGameEngine& GetEngine() {return(*m_AssociatedEngine);};
	public:
		GameObject() 
		{
			GameObject** NewPointerReference = new GameObject * (this);
			m_ObjectReferencePointer = std::shared_ptr<GameObject*>(NewPointerReference);
		};
		void AddComponent(Component* ComponentToAdd);
		virtual void Update();
		virtual void OnCreate();
		//virtual MBGameEngine& GetEngine()
		//{
		//	return(m_AssociatedEngine);
		//}
		template<typename T>
		SharedDeletableObjectReference<T> GetComponent();
		std::string const& GetName() const { return(m_Name); };
		std::string const& GetTag() const { return(m_Tag); };
		void SetName(std::string NewName) { m_Name = std::move(NewName); };
		void SetTag(std::string NewTag) { m_Tag = std::move(NewTag); };
		virtual ~GameObject()
		{
			*m_ObjectReferencePointer = nullptr;
		}
	};

	class ActiveObjectIterator
	{
		friend MBGameEngine;
	private:
		bool m_HasEnded = false;
		GameObjectReference m_CurrentReference;
		std::vector<std::unique_ptr<GameObject>>::iterator m_InternalIterator;
		std::vector<std::unique_ptr<GameObject>>::iterator m_EndIterator;
		ActiveObjectIterator() {};
	public:
		bool HasEnded();



		ActiveObjectIterator& operator++();
		ActiveObjectIterator& operator++(int);
		
		GameObjectReference& operator*();
		GameObjectReference& operator->();
		
		ActiveObjectIterator& Increment();
	};

	struct ColorRGBA
	{
		float R = 1;
		float G = 1;
		float B = 1;
		float A = 1;
	};
	class DrawObject
	{
	public:
		std::shared_ptr<Texture> Texturen = nullptr;
		ColorRGBA Color;
		//Vector2D Position;
		MBGE::Transform TextureTransform;
		float Width;
		float Height;
		std::array<int, 4> Layer = { 0,0,0,0 };
		friend bool operator<(DrawObject const& LeftObject, DrawObject const& RightObject)
		{
			bool ReturnValue = false;
			for (size_t i = 0; i < 4; i++)
			{
				if (LeftObject.Layer[i] < RightObject.Layer[i])
				{
					ReturnValue = true;
					break;
				}
				else if (LeftObject.Layer[i] > RightObject.Layer[i])
				{
					break;
				}
			}
			return(ReturnValue);
		}

		//DrawObject(std::string a, Vector2D b, float c, float d, int Layern[]);
		//DrawObject(Vector2D Start,Vector2D End ,std::vector<int> Layern);
	};
	class MBGameEngine
	{
	private:
		std::vector<std::unique_ptr<GameObject>> m_LoadedGameObjects = {};
		std::vector<std::unique_ptr<GameObject>> m_ObjectsToDelete = {};
		std::vector<std::unique_ptr<GameObject>> m_NewObjects = {};
		MBGE::MBGraphicsEngine m_InternalGraphicsEngine;

		//lite legacy grejer, inte helt säker på om jag ogillar eller gillar iden, men behövs nu i alla fall för att supporta grejerna
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_LoadedShaders = {};
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_LoadedTextures = {};

		std::vector<DrawObject> m_DrawCalls = {};


		MBGE::Camera m_SpriteCamera;
		std::unique_ptr<MBGE::SpriteModel> m_SpriteRenderingModel = nullptr;
		void p_DrawDrawObject(DrawObject& ObjectToDraw);

		void p_Update_CleanUpObjects();
		void p_Update_LoadedGameObjects();
		void p_Update_Render();
		void p_Update_Render_DrawDrawCalls();
		void p_Update();

		float m_FrameRate = 60;
	public:
		bool GetKeyDown(KeyCode KeyToCheck);
		bool GetKeyPressed(KeyCode KeyToCheck);
		bool GetKeyReleased(KeyCode KeyToCheck);
		template<typename T> GameObjectReference CreateGameObject()
		{
			m_LoadedGameObjects.push_back(std::unique_ptr<GameObject>(new T()));
			m_LoadedGameObjects.back()->m_AssociatedEngine = this;
			m_LoadedGameObjects.back()->OnCreate();
			return(GameObjectReference(m_LoadedGameObjects.back()->m_ObjectReferencePointer));
		}
		void WindowCreate(float Width,float Height,std::string const& WindowName,bool FullScreen);
		void MainLoop();

		GameObjectReference Create(GameObject* NewObject);
		void Destroy(GameObjectReference ObjectToDelete);
		GameObjectReference FindObjectWithName(std::string const& Name);
		GameObjectReference FindObjectWithTag(std::string const& ObjectTag);
		//void InitializeWindow(int Width, int Height, std::string WindowName, GLFWmonitor* Monitor, GLFWwindow* Share);


		ActiveObjectIterator GetActiveObjectsIterator();
		void ClearObjects();
		std::shared_ptr<Shader> GetNamedShader(std::string const& ShaderName);
		std::shared_ptr<Texture> GetNamedTexture(std::string const& TextureName);
		std::shared_ptr<Texture> LoadNamedTexture(std::string const& TextureName, std::string const& ResourcePath);

		std::shared_ptr<Shader> LoadShader(std::string const& ShaderName, std::string const& VertexFilepath, std::string const& FragmentFilepath);
		bool NamedTextureLoaded(std::string const& TextureName);

		void DrawTexture(std::string const& NamedTexture, MBGE::Transform TextureTransform, float Width, float Height, std::array<int, 4> Layer);
		void DrawTexture(std::shared_ptr<Texture> TextureToDraw, MBGE::Transform TextureTransform, float Width, float Height, std::array<int, 4> Layer);

		void PlaySound(std::string const& Path, float Volume);
		void PlaySound(std::string const& , float RelativeVolume, std::string const& Tag);
	};
}