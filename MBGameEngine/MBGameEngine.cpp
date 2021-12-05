#include "MBGameEngine.h"

namespace MBGameEngine
{

	//BEGIN GameObject
	void GameObject::p_DefaultUpdate()
	{
		for(std::unique_ptr<Component>& Components : m_Components)
		{
			Components->Update();
		}
	}
	void GameObject::Update()
	{

	}
	void GameObject::OnCreate()
	{

	}
	//END GameObject


	//BEGIN MBGameEngine
	void MBGameEngine::p_Update_CleanUpObjects()
	{
		//inneffektiv metod, får duga tills vidare
		while (m_ObjectsToDelete.size() > 0)
		{
			GameObjectReference ObjectToDelete = m_ObjectsToDelete.top();
			m_ObjectsToDelete.pop();
			for (int i = m_LoadedGameObjects.size()-1; i >= 0; i--)
			{
				if (m_LoadedGameObjects[i].get() == &(*ObjectToDelete))
				{
					m_LoadedGameObjects.erase(m_LoadedGameObjects.begin() + i);
				}
			}
		}
	}
	void MBGameEngine::p_Update_LoadedGameObjects()
	{
		for(std::unique_ptr<GameObject>& Object : m_LoadedGameObjects)
		{
			if (Object->m_Active == false)
			{
				continue;
			}
			Object->p_DefaultUpdate();
			Object->Update();
		}
	}
	void MBGameEngine::p_Update()
	{
		p_Update_LoadedGameObjects();
	}
	bool MBGameEngine::GetKeyDown(KeyCode KeyToCheck)
	{
		return(false);
	}
	bool MBGameEngine::GetKeyPressed(KeyCode KeyToCheck)
	{
		return(false);
	}
	bool MBGameEngine::GetKeyReleased(KeyCode KeyToCheck)
	{
		return(false);
	}
	void MBGameEngine::Destroy(GameObjectReference ObjectToDelete)
	{
		if (ObjectToDelete)
		{
			m_ObjectsToDelete.push(ObjectToDelete);
			ObjectToDelete->m_Active = false;
		}
	}
	GameObjectReference MBGameEngine::Create(GameObject* NewObject)
	{
		m_LoadedGameObjects.push_back(std::unique_ptr<GameObject>(NewObject));
		NewObject->m_AssociatedEngine = this;
		NewObject->OnCreate();
		return(GameObjectReference(m_LoadedGameObjects.back()->m_ObjectReferencePointer));
	}
	void MBGameEngine::MainLoop()
	{
			
	}
	void MBGameEngine::PlaySound(std::string const& SoundFile, float Volume)
	{

	}

	GameObjectReference MBGameEngine::FindObjectWithName(std::string const& Name)
	{
		GameObjectReference ReturnValue;
		for (std::unique_ptr<GameObject>& Objects : m_LoadedGameObjects)
		{
			if (Objects->GetName() == Name && Objects->m_Active)
			{
				ReturnValue = GameObjectReference(Objects->m_ObjectReferencePointer);
				break;
			}
		}
		return(ReturnValue);
	}
	GameObjectReference MBGameEngine::FindObjectWithTag(std::string const& Name)
	{
		GameObjectReference ReturnValue;
		for (std::unique_ptr<GameObject>& Objects : m_LoadedGameObjects)
		{
			if (Objects->GetTag() == Name && Objects->m_Active)
			{
				ReturnValue = GameObjectReference(Objects->m_ObjectReferencePointer);
				break;
			}
		}
		return(ReturnValue);
	}
	//END MBGameEngine
}