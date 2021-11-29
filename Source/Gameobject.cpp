#include <Gameobject.h>
#include <VertexBuffer.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <ctime>
#include <iostream>
#include <IndexBuffer.h>
#include <Texture.h>
#include <GLFW/glfw3.h>
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
	int Window_Height;
	int Window_Width;
	//TODO Ha koordinater i vår engine som vi kan få fram, utan att behöva calla denn funktion varje gång, vem vet hur många cycles den tar
	//glfwGetWindowSize(TouhouEngine::CurrentWindow, &Window_Width, &Window_Height);
	TouhouEngine::_GetWindowSize(&Window_Width, &Window_Height);
	float CameraCoordinates_X = Position.x/8;
	float CameraCoordinates_Y = Position.y/4.5;
	float Sprite_Width = Renderer.Size/8;
	float Sprite_Height = Renderer.Size*((float)Renderer.ObjectTexture.GetHeight() / (float)Renderer.ObjectTexture.GetWidth())/8;

	Vector2D Vertex1( - Sprite_Width / 2, - Sprite_Height / 2);
	Vector2D Vertex2( + Sprite_Width / 2, - Sprite_Height / 2);
	Vector2D Vertex3( + Sprite_Width / 2, + Sprite_Height / 2);
	Vector2D Vertex4( - Sprite_Width / 2, + Sprite_Height / 2);

	Vertex1.Rotate(Rotation);
	Vertex2.Rotate(Rotation);
	Vertex3.Rotate(Rotation);
	Vertex4.Rotate(Rotation);

	float ScreenRelation = ((float)Window_Width / (float)Window_Height);
	float positions[]
	{
		CameraCoordinates_X + Vertex1.x,CameraCoordinates_Y + Vertex1.y*ScreenRelation, 0.0f,0.0f,
		CameraCoordinates_X + Vertex2.x,CameraCoordinates_Y + Vertex2.y*ScreenRelation, 1.0f,0.0f,
		CameraCoordinates_X + Vertex3.x,CameraCoordinates_Y + Vertex3.y*ScreenRelation,1.0f,1.0f,
		CameraCoordinates_X + Vertex4.x,CameraCoordinates_Y + Vertex4.y*ScreenRelation,0.0f,1.0f
	};
	unsigned int indices[] =
	{
		0,1,2,
		2,3,0
	};	

	//TODO Fixa render koden i fienderna så att vi kan ha en gemensam vertex array och buffer istället för att skapa nya varje gång
	VertexArray va;
	VertexBuffer vb(positions, 4 * 4 * sizeof(float));
	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);
	va.AddBuffer(vb, layout);
	IndexBuffer ib(indices, 6);
	ib.Bind();

	Renderer.ObjectTexture.Bind();

	//rendern att använda
	auto ShaderToUse = TouhouEngine::LoadedShaders["SpriteShader"];
	ShaderToUse->Bind();
	ShaderToUse->SetUniform1i("u_Texture", 0);
	ShaderToUse->SetUniform4f("ColorKoef", this->Renderer.ColorKoef.R, this->Renderer.ColorKoef.G, this->Renderer.ColorKoef.B, this->Renderer.ColorKoef.A);

	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

	//*/
	//debug grej som gör att vi ritar själva strecken som utgör hitboxen
	if (Debug)
	{
		std::vector<int> Layern = { 1000,0,0,0 };
		Vector2D HitboxVertex1 = Vector2D(-Hitbox.x / 2, -Hitbox.y / 2);
		Vector2D HitboxVertex2 = Vector2D(-Hitbox.x / 2, +Hitbox.y / 2);
		Vector2D HitboxVertex3 = Vector2D(+Hitbox.x / 2, +Hitbox.y / 2);
		Vector2D HitboxVertex4 = Vector2D(+Hitbox.x / 2, -Hitbox.y / 2);
		HitboxVertex1.Rotate(Rotation);
		HitboxVertex2.Rotate(Rotation);
		HitboxVertex3.Rotate(Rotation);
		HitboxVertex4.Rotate(Rotation);
		HitboxVertex1 =HitboxVertex1+ Position;
		HitboxVertex2 =HitboxVertex2+ Position;
		HitboxVertex3 =HitboxVertex3+ Position;
		HitboxVertex4 =HitboxVertex4+ Position;
		
		Texture::DrawLine(HitboxVertex1, HitboxVertex2, Layern);
		Texture::DrawLine(HitboxVertex1, HitboxVertex4, Layern);
		Texture::DrawLine(HitboxVertex3, HitboxVertex2, Layern);
		Texture::DrawLine(HitboxVertex3, HitboxVertex4, Layern);
	}
}
GameObjectRenderer::~GameObjectRenderer()
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
GameObject::GameObject(std::string Namn,float Storlek) : Renderer(Namn,Storlek)
{
}
GameObjectRenderer::GameObjectRenderer(std::string Namn, float Storlek) :Image(Namn),Size(Storlek)
{
	ColorKoef.A = 1;
	ColorKoef.R = 1;
	ColorKoef.B = 1;
	ColorKoef.G = 1;
	if (TouhouEngine::LoadedTextures.find(Namn) == TouhouEngine::LoadedTextures.end())
	{
		Texture* NyTexture = new Texture("Resources/SpelResurser/Sprites/" + Namn);
		ObjectTexture = *NyTexture;
		TouhouEngine::LoadedTextures[Namn] = NyTexture;
	}
	else
	{
		ObjectTexture = *TouhouEngine::LoadedTextures[Namn];
	}
}
/*constructorn innan jag lade in caching systemet
GameObjectRenderer::GameObjectRenderer(std::string Namn, float Storlek) : ObjectTexture("SpelResurser/Sprites/" + Namn),Image(Namn),Size(Storlek)
{

}
*/
void GameObject::Collision()
{
	//nu kommer vi till problemet av att ha en kollision för spelet. Ett sätt vore att varje föremål checkar kollision mot alla andra föremål i rummet
	//men då vår vi problemet att antalet jämförelser växer med n^2 eller i bästa fall (n^2-n)/2. om det är säg 100 kulor blir det 100 jämförelser.
	//frågan om det ens går att göra på ett bättre sätt. En grej man skulle kunna göra är att innan man kör någon mer logic så kollar man att avståndet
	//mellan dem 2 föremålen är ett minsta värde, om det inte är det kan vi ju direkt avgöra att det inte kan vara en kollision.
	//En annan metod vore att kolla Kollision för enbart specifika grejer. I vårt fall kan ju alla fiende kulor bara kolla om dem kommer träffa en spelare
	//och om dem gör det så händer saker. Våra kulor har ju fortfarande problemet. Vi börjar kollision med fiend kulor och oss
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