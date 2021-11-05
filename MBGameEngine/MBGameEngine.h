#pragma once
#include <vector>
#include <memory>
#include <string>
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


	public:

	};

	class Texture
	{
	private:

	public:

	};

	class MBGameEngine;
	class GameObject
	{
	private:
		std::string m_Name = "";
		std::string m_Tag = "";
		MBGameEngine m_AssociatedEngine = nullptr;
		std::vector<std::unique_ptr<Component>> m_Components = {};
	public:
		virtual void Update();
	};

	class GameObjectReference
	{

	};

	class MBGameEngine
	{
	private:

	public:
		bool GetKeyDown(KeyCode KeyToCheck);
		bool GetKeyPressed(KeyCode KeyToCheck);
		bool GetKeyReleased(KeyCode KeyToCheck);
		template<typename T> GameObjectReference CreateGameObject()
		{

		}
		void DestroyGameObject(GameObjectReference ObjectToDelete);
	};
}