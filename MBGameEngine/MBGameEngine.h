#pragma once
#include <vector>
#include <memory>
#include <string>
#include <vector>
#include <stack>
namespace MBGE
{
	enum class KeyCode
	{
		A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
		Shift,Left,Right,Up,Down,
	};


	class Component
	{
	private:

		GameObjectReference m_AssociatedGameObject;
		std::shared_ptr<Component*> m_ComponentReference = nullptr;

	public:
		virtual void Update();
		virtual ~Component();
	};

	class Texture
	{
	private:

	public:

	};

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
		operator bool()
		{
			return(*m_ObjectPointer != nullptr);
		}
		bool operator==(T* OtherPointer)
		{
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
	}



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
	public:
		GameObject() 
		{
			m_ObjectReferencePointer = std::make_shared(this);
		};
		virtual void Update();
		virtual void OnCreate();
		virtual MBGameEngine& GetEngine()
		{
			return(m_AssociatedEngine);
		}

		std::string GetName() const { return(m_Name) };
		std::string GetTag() const { return(m_Tag) };
		virtual ~GameObject()
		{
			*m_ObjectReferencePointer = nullptr;
		}
	};

	typedef SharedDeletableObjectReference<GameObject> GameObjectReference;
	typedef SharedDeletableObjectReference<Component> ComponentReference;

	class MBGameEngine
	{
	private:
		std::vector<std::unique_ptr<GameObject>> m_LoadedGameObjects = {};
		std::stack<GameObjectReference> m_ObjectsToDelete = {};

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
			m_LoadedGameObjects.back()->OnCreate();
			m_LoadedGameObjects.back()->m_AssociatedEngine = this;
			return(GameObjectReference(m_LoadedGameObjects.back()->m_ObjectReferencePointer));
		}
		void DestroyGameObject(GameObjectReference ObjectToDelete);

		//Top Loop grejer
		void MainLoop();

		//convinience icke best practice grejer
		void PlaySound(std::string const& SoundFile, float Volume);
		GameObjectReference FindGameObjectWithName(std::string const& Name);
		GameObjectReference FindGameObjectWithTag(std::string const& Name);
	};
}