#include <Gameobject.h>
#include <ctime>
#include <iostream>
#include <Engine.h>
//extern GLFWwindow* window;
//extern std::vector<GameObject*> ActiveGameObjects;
//extern std::unordered_map<std::string, Texture*> LoadedTextures;
bool Debug = false;
SpriteRenderer::SpriteRenderer()
{
	ColorKoef.A = 1;
	ColorKoef.R = 1;
	ColorKoef.B = 1;
	ColorKoef.G = 1;
}
void SpriteRenderer::Update()
{
	if (SpriteTexture == nullptr)
	{
		return;
	}
	float Sprite_Width = Width;
	float Sprite_Height = Sprite_Width * ((float)SpriteTexture->GetHeight() / (float)SpriteTexture->GetWidth());
	TouhouEngine::DrawTexture(SpriteTexture, GetGameObject()->Transform.GetPosition(), Sprite_Width, Sprite_Height, Layer);
}
//GameObjectRenderer::GameObjectRenderer(std::string Bild)
//{
//	ColorKoef.A = 1;
//	ColorKoef.R = 1;
//	ColorKoef.B = 1;
//	ColorKoef.G = 1;
//}
GameObject::GameObject()
{	/*
	Shader Shadern("Source/vertexshader.txt", "Source/fragmentshader.txt");
	Renderer.ObjectShader = Shadern;
	Renderer.ObjectShader.Bind();
	Renderer.ObjectShader.SetUniform1i("u_Texture", 0);
	Renderer.ObjectTexture = Texture("SpelResurser/Sprites/" + Renderer.Image);
	*/
}
//GameObject::GameObject(Vector2D Plats, std::string Namn, std::string Tagg)
//	:Position(Plats),Name(Namn),Tag(Tagg),Renderer(SpriteRenderer())
//{
//	if (!TouhouEngine::NamedTextureLoaded(Namn))
//	{
//		//Texture* NyTexture = new Texture("Resources/SpelResurser/Sprites/" + Namn);
//		//ObjectTexture = *NyTexture;
//		//TouhouEngine::LoadedTextures[Namn] = NyTexture;
//		Renderer.SpriteTexture = TouhouEngine::LoadNamedTexture(Namn, "Resources/SpelResurser/Sprites/" + Namn);
//	}
//	else
//	{
//		Renderer.SpriteTexture = TouhouEngine::GetNamedTexture(Namn);
//	}
//	//Renderer.Width = Storlek;
//}
void GameObject::Update()
{

}
void GameObject::OnCreate()
{

}
//void GameObject::Render()
//{
//	if (Renderer.SpriteTexture == nullptr)
//	{
//		return;
//	}
//	float Sprite_Width = Renderer.Width;
//	float Sprite_Height = Sprite_Width *((float)Renderer.SpriteTexture->GetHeight() / (float)Renderer.SpriteTexture->GetWidth());
//	std::array<int,4> Layer;
//	Layer[0] = Renderer.Layer[0];
//	Layer[1] = Renderer.Layer[1];
//	Layer[2] = Renderer.Layer[2];
//	Layer[3] = Renderer.Layer[3];
//	TouhouEngine::DrawTexture(Renderer.SpriteTexture, Position, Sprite_Width, Sprite_Height, Layer);
//}
SpriteRenderer::~SpriteRenderer()
{

}
GameObject::~GameObject()
{
	//std::cout << Name << " Forstordes" << std::endl;
	//vi vill förstöra alla komponenter då dem alla är dynamiskt allokerade
	for (int i = 0; i < Components.size(); i++)
	{
		delete(Components[i]);
	}
}
//GameObject::GameObject(std::string Namn,float Storlek)
//{
//	if (!TouhouEngine::NamedTextureLoaded(Namn))
//	{
//		//Texture* NyTexture = new Texture("Resources/SpelResurser/Sprites/" + Namn);
//		//ObjectTexture = *NyTexture;
//		//TouhouEngine::LoadedTextures[Namn] = NyTexture;
//		Renderer.SpriteTexture = TouhouEngine::LoadNamedTexture(Namn, "Resources/SpelResurser/Sprites/" + Namn);
//	}
//	else
//	{
//		Renderer.SpriteTexture = TouhouEngine::GetNamedTexture(Namn);
//	}
//	Renderer.Width = Storlek;
//}
//GameObjectRenderer::GameObjectRenderer(std::string Namn, float Storlek) :Image(Namn),Width(Storlek)
//{
//	ColorKoef.A = 1;
//	ColorKoef.R = 1;
//	ColorKoef.B = 1;
//	ColorKoef.G = 1;
//	if (!TouhouEngine::NamedTextureLoaded(Namn))
//	{
//		//Texture* NyTexture = new Texture("Resources/SpelResurser/Sprites/" + Namn);
//		//ObjectTexture = *NyTexture;
//		//TouhouEngine::LoadedTextures[Namn] = NyTexture;
//		ObjectTexture = TouhouEngine::LoadNamedTexture(Namn, "Resources/SpelResurser/Sprites/" + Namn);
//	}
//	else
//	{
//		ObjectTexture = TouhouEngine::GetNamedTexture(Namn);
//	}
//}
/*constructorn innan jag lade in caching systemet
GameObjectRenderer::GameObjectRenderer(std::string Namn, float Storlek) : ObjectTexture("SpelResurser/Sprites/" + Namn),Image(Namn),Size(Storlek)
{

}
*/

//component grejer
void GameObject::UpdateComponents()
{
	for (size_t i = 0; i < Components.size(); i++)
	{
		Components[i]->Update();
	}
}
void GameObject::AddComponent(Component* Komponenten)
{
	Components.push_back(Komponenten);
	Komponenten->SpelObjectet = this;
	//ComponentDictionary[Komponenten->ComponentName] = Komponenten;
}
//Component* GameObject::GetComponent(std::string ComponentName)
//{
//	if(ComponentDictionary.find(ComponentName) != ComponentDictionary.end())
//	{
//		return(ComponentDictionary[ComponentName]);
//	}
//	else
//	{
//		return(nullptr);
//	}
//}
