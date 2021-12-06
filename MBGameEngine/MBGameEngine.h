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
	//enum class KeyCode
	//{
	//	A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
	//	LeftShift,Space,Left,Right,Up,Down,Esc,Enter,
	//};
	typedef MBGE::KeyCode KeyCode;
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
		template<typename U>
		friend class SharedDeletableObjectReference;
		T* m_ObjectPointer = nullptr;
		std::shared_ptr<bool> m_ObjectDeleted = nullptr;
		void p_Validate() const
		{
			if (m_ObjectPointer == nullptr || !(m_ObjectDeleted.get()!= nullptr && *m_ObjectDeleted==false))
			{
				throw std::exception();
			}
		}
	public:
		SharedDeletableObjectReference(T* ObjectPointer,std::shared_ptr<bool> DeletedPointer)
		{
			m_ObjectPointer = ObjectPointer;
			m_ObjectDeleted = DeletedPointer;
		}
		//template<typename M>
		//operator SharedDeletableObjectReference<M>()
		//{
		//	//std::dynamic_pointer_cast<M*,T*>(m_ObjectPointer)
		//	//static_assert(1 == 2,std::typeid(M).name());
		//	SharedDeletableObjectReference<M> ReturnValue(m_ObjectPointer, m_ObjectDeleted);
		//	return(ReturnValue);
		//
		//}
		template<typename M>
		SharedDeletableObjectReference(SharedDeletableObjectReference<M> Object)
		{
			std::swap(m_ObjectDeleted, Object.m_ObjectDeleted);
			m_ObjectPointer = dynamic_cast<T*>(Object.m_ObjectPointer);
		}
		template<typename M>
		SharedDeletableObjectReference& operator=(SharedDeletableObjectReference<M> Object)
		{
			std::swap(m_ObjectDeleted, Object.m_ObjectDeleted);
			m_ObjectPointer = dynamic_cast<T*>(Object.m_ObjectPointer);
			return(*this);
		}
		SharedDeletableObjectReference()
		{

		}
		operator bool()
		{
			return(m_ObjectPointer != nullptr && (m_ObjectDeleted.get() != nullptr && *m_ObjectDeleted == false));
		}
		friend bool operator==(SharedDeletableObjectReference const& LeftObject, const T* RightPointer)
		{
			return(LeftObject.m_ObjectPointer == RightPointer);
		}
		friend bool operator==(const T* LeftPointer, SharedDeletableObjectReference const& RightObject)
		{
			return(RightObject, LeftPointer);
		}
		friend bool operator!=(SharedDeletableObjectReference const& LeftObject, const T* RightPointer)
		{
			return(!(LeftObject == RightPointer));
		}
		friend bool operator!=(const T* LeftPointer, SharedDeletableObjectReference const& RightObject)
		{
			return(!(LeftPointer == RightObject));
		}
		T* operator->()
		{
			p_Validate();
			return m_ObjectPointer;
		}
		T& operator*()
		{
			p_Validate();
			return *m_ObjectPointer;
		}
		T const* operator->() const
		{
			p_Validate();
			return m_ObjectPointer;
		}
		T const& operator*() const
		{
			p_Validate();
			return *m_ObjectPointer;
		}
	};
	// template <typename T>
	// using SharedDeletableObjectReference<GameObject> = GameObjectReference;
	class GameObject;
	class Component;
	typedef SharedDeletableObjectReference<Component> ComponentReference;
	template <typename T>
	using ObjectReference = SharedDeletableObjectReference<T>;
	typedef ObjectReference<GameObject> GameObjectReference;

	class MBGameEngine;
	class Component
	{
	private:
		friend class GameObject;
		GameObjectReference m_AssociatedGameObject;
		std::shared_ptr<bool> m_ComponentDeleted = nullptr;
	protected:	
	MBGameEngine& GetEngine();
	public:
		Component()
		{
			m_ComponentDeleted = std::make_shared<bool>(false);
		}
		GameObjectReference GetGameObject();
		const GameObjectReference GetGameObject() const;
		//GameObjectReference GetGameObject() const;
		virtual void Update();
		virtual ~Component();
	};

	class GameObject
	{
	private:
		friend class MBGameEngine;
		friend class ActiveObjectIterator;
		std::string m_Name = "";
		std::string m_Tag = "";
		bool m_Active = true;


		std::shared_ptr<bool> m_ObjectDeletedPointer = {};
		MBGameEngine* m_AssociatedEngine = nullptr;
		std::vector<std::unique_ptr<Component>> m_Components = {};
		void p_DefaultUpdate();
		//GameObject(MBGameEngine* AssociatedEngine);
		bool m_Deleted = false;
		//bool IsDeleted();
	protected:
	public:
		MBGameEngine& GetEngine() { return(*m_AssociatedEngine); };
		bool Active = true;
		MBGE::Transform Transform;

		GameObject() 
		{
			///GameObject** NewPointerReference = new GameObject * (this);
			m_ObjectDeletedPointer = std::make_shared<bool>(false);
		};
		GameObjectReference GetReference()
		{
			return(GameObjectReference(this, m_ObjectDeletedPointer));
		}
		void AddComponent(Component* ComponentToAdd);
		virtual void Update();
		virtual void OnCreate();
		//virtual MBGameEngine& GetEngine()
		//{
		//	return(m_AssociatedEngine);
		//}
		template<typename T>
		ObjectReference<T> GetComponent()
		{
			ObjectReference<T> ReturnValue;
			for (size_t i = 0; i < m_Components.size(); i++)
			{
				T* ComponentPointer = dynamic_cast<T*>(m_Components[i].get());
				if (ComponentPointer != nullptr)
				{
					ReturnValue = ObjectReference<T>(ComponentPointer, ComponentPointer->m_ComponentDeleted);
					break;
				}
			}
			return(ReturnValue);
		}
		std::string const& GetName() const { return(m_Name); };
		std::string const& GetTag() const { return(m_Tag); };
		void SetName(std::string NewName) { m_Name = std::move(NewName); };
		void SetTag(std::string NewTag) { m_Tag = std::move(NewTag); };
		virtual ~GameObject()
		{
			*m_ObjectDeletedPointer = true;
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
				if (LeftObject.Layer[i] > RightObject.Layer[i])
				{
					ReturnValue = true;
					break;
				}
				else if (LeftObject.Layer[i] < RightObject.Layer[i])
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

		void Destroy(GameObjectReference ObjectToDelete);
		void Destroy(GameObject* ObjectToDelete);
		GameObjectReference Create(GameObject* NewObject);
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