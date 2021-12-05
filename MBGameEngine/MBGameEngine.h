#pragma once
#include <vector>
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <MBGraphicsEngine/MBGE.h>
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

	class Component
	{
	private:

		GameObjectReference m_AssociatedGameObject;
		std::shared_ptr<Component*> m_ComponentReference = nullptr;

	public:
		virtual void Update();
		virtual ~Component();
	};

	class MBGameEngine;
	class GameObject
	{
	private:
		friend class MBGameEngine;
		std::string m_Name = "";
		std::string m_Tag = "";
		bool m_Active = true;


		std::shared_ptr<GameObject*> m_ObjectReferencePointer = {};
		MBGameEngine* m_AssociatedEngine = nullptr;
		std::vector<std::unique_ptr<Component>> m_Components = {};
		void p_DefaultUpdate();
		//GameObject(MBGameEngine* AssociatedEngine);
	protected:
		MBGameEngine& GetEngine() {return(*m_AssociatedEngine);};
	public:
		GameObject() 
		{
			GameObject** NewPointerReference = new GameObject * (this);
			m_ObjectReferencePointer = std::shared_ptr<GameObject*>(NewPointerReference);
		};
		virtual void Update();
		virtual void OnCreate();
		//virtual MBGameEngine& GetEngine()
		//{
		//	return(m_AssociatedEngine);
		//}
		template<typename T>
		SharedDeletableObjectReference<T> GetComponent();
		std::string GetName() const { return(m_Name); };
		std::string GetTag() const { return(m_Tag); };
		virtual ~GameObject()
		{
			*m_ObjectReferencePointer = nullptr;
		}
	};

	class ActiveObjectIterator
	{
		friend MBGameEngine;
	private:


	public:
		bool HasEnded();



		ActiveObjectIterator& operator++();
		ActiveObjectIterator& operator++(int);
		
		GameObjectReference& operator*();
		GameObjectReference& operator->();
		
		ActiveObjectIterator& Increment();
	};
	class MBGameEngine
	{
	private:
		std::vector<std::unique_ptr<GameObject>> m_LoadedGameObjects = {};
		std::stack<GameObjectReference> m_ObjectsToDelete = {};
		MBGE::MBGraphicsEngine m_InternalGraphicsEngine;
		void p_Update_CleanUpObjects();
		void p_Update_LoadedGameObjects();
		void p_Update();
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
		bool NamedTextureLoaded(std::string const& TextureName);

		//void DrawTexture(std::string const& NamedTexture, Vector2D Position, float Width, float Height, std::array<int, 4> Layer);
		//void DrawTexture(std::shared_ptr<Texture> TextureToDraw, Vector2D Position, float Width, float Height, std::array<int, 4> Layer);

		std::shared_ptr<Shader> LoadShader(std::string ShaderName, std::string VertexFilepath, std::string FragmentFilepath);

		void PlaySound(std::string const& Path, float Volume);
		void PlaySound(std::string const& , float RelativeVolume, std::string const& Tag);
	};
}