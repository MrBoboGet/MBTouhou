#include "MBGameEngine.h"

namespace MBGE
{

	//BEGIN GameObject
	void GameObject::p_DefaultUpdate()
	{
		for(Component& Components : m_Components)
		{
			Components.Update();
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
		//inneffektiv metod, f�r duga tills vidare
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
		for(GameObject& Object : m_LoadedGameObjects)
		{
			if (Object.m_Active == false)
			{
				continue;
			}
			Object.p_DefaultUpdate();
			Object.Update();
		}
	}
	void MBGameEngine::p_Update()
	{
		p_Update_LoadedGameObjects();
	}
	bool MBGameEngine::GetKeyDown(KeyCode KeyToCheck)
	{

	}
	bool MBGameEngine::GetKeyPressed(KeyCode KeyToCheck)
	{

	}
	bool MBGameEngine::GetKeyReleased(KeyCode KeyToCheck)
	{

	}
	void MBGameEngine::DestroyGameObject(GameObjectReference ObjectToDelete)
	{
		if (ObjectToDelete)
		{
			m_ObjectsToDelete.push(ObjectToDelete);
			ObjectToDelete->m_Active = false;
		}
	}
	void MBGameEngine::MainLoop()
	{
			
	}
	void MBGameEngine::PlaySound(std::string const& SoundFile, float Volume)
	{

	}

	GameObjectReference MBGameEngine::FindGameObjectWithName(std::string const& Name)
	{
		GameObjectReference ReturnValue;
		for (GameObject& Objects : m_LoadedGameObjects)
		{
			if (Objects.GetName() == Name && Objects.m_Active)
			{
				ReturnValue = GameObjectReference(Objects.m_ObjectReferencePointer);
				break;
			}
		}
		return(ReturnValue);
	}
	GameObjectReference MBGameEngine::FindGameObjectWithTag(std::string const& Name)
	{
		GameObjectReference ReturnValue;
		for (GameObject& Objects : m_LoadedGameObjects)
		{
			if (Objects.GetTag() == Name && Objects.m_Active)
			{
				ReturnValue = GameObjectReference(Objects.m_ObjectReferencePointer);
				break;
			}
		}
		return(ReturnValue);
	}
	//END MBGameEngine
}