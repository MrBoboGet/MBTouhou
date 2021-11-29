#pragma once
#include <Vector2D.h>
#include <string>
#include <Renderer.h>
#include <VertexBufferLayout.h>
#include <Texture.h>
#include <unordered_map>
#include <Engine/Engine_GenericComponent.h>
#include <memory>
//iomed att jag �r lite lat och vet att vi inte igentligen beh�ver �ndra shadern s� kommer jag fr�n och med nu antat att varje objekt inte beh�ver en unik
class GameObjectRenderer
{
public:
	struct ColorRGBA
	{
		float R;
		float G;
		float B;
		float A;
	};
	ColorRGBA ColorKoef;
	std::string Image;
	float Size;
	std::vector<int> Layer = { 0,0,0,0 };
	//ett problem vi f�r med den h�r koden �r att varje gameobject g�r sin egen kopia av samma texture, vilket tar minne och eventuellt prestanda
	//tanken �r att jag g�r en dictionary s� att om en texture redans finns s� s�ger vi att render idn �r den texturen
	std::shared_ptr<Texture> ObjectTexture = nullptr;
	//Shader ObjectShader;
	GameObjectRenderer(std::string Bild);
	GameObjectRenderer(std::string Namn, float Storlek);
	GameObjectRenderer();
	~GameObjectRenderer();
};
class GameObject
{
protected:
	std::string Name;
	std::string Tag;
	std::vector<Component*> Components = std::vector<Component*>(0);
	std::unordered_map<std::string, Component*> ComponentDictionary = std::unordered_map<std::string,Component*>(0);
public:
	Vector2D Position;
	int HP;
	float Rotation = 0;
	Vector2D Hitbox;
	GameObjectRenderer Renderer;
	bool Active = true;

	//komponent grejer
	void UpdateComponents();
	void AddComponent(Component* ComponentName);
	Component* GetComponent(std::string ComponentName);

	virtual void Update();
	virtual void Collision();
	void Render();
	GameObject();
	GameObject(Vector2D Plats, std::string Namn, std::string Tagg);
	GameObject(std::string Namn,float Storlek);
	virtual ~GameObject();
	inline std::string GetTag()
	{
		return(Tag);
	}
	inline std::string GetName()
	{
		return(Name);
	}
	//n�gon form av rendering funktion, eller �r det n�got vi kan l�gga till efter? 
	//man skulle kunna g�ra update och liknande virtual, och ha grundl�ggande grejer i dem s� att man alltid vet att dem har en
};
