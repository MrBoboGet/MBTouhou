#pragma once
#include <Vector2D.h>
#include <string>
#include <Renderer.h>
#include <VertexBufferLayout.h>
#include <Texture.h>
#include <unordered_map>
#include <Engine/Engine_GenericComponent.h>
#include <memory>
//iomed att jag är lite lat och vet att vi inte igentligen behöver ändra shadern så kommer jag från och med nu antat att varje objekt inte behöver en unik
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
	//ett problem vi får med den här koden är att varje gameobject gör sin egen kopia av samma texture, vilket tar minne och eventuellt prestanda
	//tanken är att jag gör en dictionary så att om en texture redans finns så säger vi att render idn är den texturen
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
	//någon form av rendering funktion, eller är det något vi kan lägga till efter? 
	//man skulle kunna göra update och liknande virtual, och ha grundläggande grejer i dem så att man alltid vet att dem har en
};
