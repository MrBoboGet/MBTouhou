#include <Gameobject.h>
#include <VertexBuffer.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <ctime>
#include <iostream>
#include <IndexBuffer.h>
#include <Texture.h>
#include <Engine.h>
//extern GLFWwindow* window;
//extern std::vector<GameObject*> ActiveGameObjects;
//extern std::unordered_map<std::string, Texture*> LoadedTextures;
bool Debug = false;
GameObjectRenderer::GameObjectRenderer()
{
	ColorKoef.A = 1;
	ColorKoef.R = 1;
	ColorKoef.B = 1;
	ColorKoef.G = 1;
}
GameObjectRenderer::GameObjectRenderer(std::string Bild)
{
	ColorKoef.A = 1;
	ColorKoef.R = 1;
	ColorKoef.B = 1;
	ColorKoef.G = 1;
}
GameObject::GameObject()
{	/*
	Shader Shadern("Source/vertexshader.txt", "Source/fragmentshader.txt");
	Renderer.ObjectShader = Shadern;
	Renderer.ObjectShader.Bind();
	Renderer.ObjectShader.SetUniform1i("u_Texture", 0);
	Renderer.ObjectTexture = Texture("SpelResurser/Sprites/" + Renderer.Image);
	*/
}
GameObject::GameObject(Vector2D Plats, std::string Namn, std::string Tagg)
	:Position(Plats),Name(Namn),Tag(Tagg),Renderer(GameObjectRenderer())
{

}
void GameObject::Update()
{

}
void GameObject::Render()
{
	if (Renderer.ObjectTexture == nullptr)
	{
		return;
	}
	int Window_Height;
	int Window_Width;
	//TODO Ha koordinater i v�r engine som vi kan f� fram, utan att beh�va calla denn funktion varje g�ng, vem vet hur m�nga cycles den tar
	//glfwGetWindowSize(TouhouEngine::CurrentWindow, &Window_Width, &Window_Height);
	TouhouEngine::_GetWindowSize(&Window_Width, &Window_Height);
	float Sprite_Width = Renderer.Size;
	float Sprite_Height = Sprite_Width *((float)Renderer.ObjectTexture->GetHeight() / (float)Renderer.ObjectTexture->GetWidth());
	int Layer[4];
	Layer[0] = Renderer.Layer[0];
	Layer[1] = Renderer.Layer[1];
	Layer[2] = Renderer.Layer[2];
	Layer[3] = Renderer.Layer[3];
	TouhouEngine::DrawTexture(Renderer.ObjectTexture, Position, Sprite_Width, Sprite_Height, Layer);

}
GameObjectRenderer::~GameObjectRenderer()
{

}
GameObject::~GameObject()
{
	//std::cout << Name << " Forstordes" << std::endl;
	//vi vill f�rst�ra alla komponenter d� dem alla �r dynamiskt allokerade
	for (int i = 0; i < Components.size(); i++)
	{
		delete(Components[i]);
	}
}
GameObject::GameObject(std::string Namn,float Storlek) : Renderer(Namn,Storlek)
{
}
GameObjectRenderer::GameObjectRenderer(std::string Namn, float Storlek) :Image(Namn),Size(Storlek)
{
	ColorKoef.A = 1;
	ColorKoef.R = 1;
	ColorKoef.B = 1;
	ColorKoef.G = 1;
	if (!TouhouEngine::NamedTextureLoaded(Namn))
	{
		//Texture* NyTexture = new Texture("Resources/SpelResurser/Sprites/" + Namn);
		//ObjectTexture = *NyTexture;
		//TouhouEngine::LoadedTextures[Namn] = NyTexture;
		ObjectTexture = TouhouEngine::LoadNamedTexture(Namn, "Resources/SpelResurser/Sprites/" + Namn);
	}
	else
	{
		ObjectTexture = TouhouEngine::GetNamedTexture(Namn);
	}
}
/*constructorn innan jag lade in caching systemet
GameObjectRenderer::GameObjectRenderer(std::string Namn, float Storlek) : ObjectTexture("SpelResurser/Sprites/" + Namn),Image(Namn),Size(Storlek)
{

}
*/
void GameObject::Collision()
{
	//nu kommer vi till problemet av att ha en kollision f�r spelet. Ett s�tt vore att varje f�rem�l checkar kollision mot alla andra f�rem�l i rummet
	//men d� v�r vi problemet att antalet j�mf�relser v�xer med n^2 eller i b�sta fall (n^2-n)/2. om det �r s�g 100 kulor blir det 100 j�mf�relser.
	//fr�gan om det ens g�r att g�ra p� ett b�ttre s�tt. En grej man skulle kunna g�ra �r att innan man k�r n�gon mer logic s� kollar man att avst�ndet
	//mellan dem 2 f�rem�len �r ett minsta v�rde, om det inte �r det kan vi ju direkt avg�ra att det inte kan vara en kollision.
	//En annan metod vore att kolla Kollision f�r enbart specifika grejer. I v�rt fall kan ju alla fiende kulor bara kolla om dem kommer tr�ffa en spelare
	//och om dem g�r det s� h�nder saker. V�ra kulor har ju fortfarande problemet. Vi b�rjar kollision med fiend kulor och oss
}

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
	ComponentDictionary[Komponenten->ComponentName] = Komponenten;
}
Component* GameObject::GetComponent(std::string ComponentName)
{
	if(ComponentDictionary.find(ComponentName) != ComponentDictionary.end())
	{
		return(ComponentDictionary[ComponentName]);
	}
	else
	{
		return(nullptr);
	}
}