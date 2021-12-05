#include<Enemy_Bullet_Template.h>
#include <Hitbox.h>
#include <SpelObjekt.h>
Enemy_Bullet_Template::~Enemy_Bullet_Template()
{

}
Enemy_Bullet_Template::Enemy_Bullet_Template(Vector2D Plats, std::string Namn, std::string Tagg, std::string Bild, float Storlek, Vector2D Hitplox, void(*Funktion)(Enemy_Bullet_Template*))// : GameObject(Bild,Storlek)
{
	AddComponent(new SpriteRenderer());
	AddComponent(new Rectangle_Hitbox());
	AddComponent(new MBTouhou_Bullet_DeleteOffScreen());
	//Position = Plats;
	Transform.SetPosition(Plats);
	UpdateFunction = Funktion;
	SetName(Namn);
	SetTag(Tagg);
	GetComponent<Rectangle_Hitbox>()->Width =Hitplox.x;
	GetComponent<Rectangle_Hitbox>()->Height =Hitplox.y;
	GetComponent<SpriteRenderer>()->Width = Storlek;
	m_TextureName = Bild;
	//
}
void Enemy_Bullet_Template::OnCreate()
{
	SpriteRenderer* Renderer = GetComponent<SpriteRenderer>();
	Renderer->SpriteTexture = TouhouEngine::LoadNamedTexture(m_TextureName, "Resources/SpelResurser/Sprites/" + m_TextureName);
}
void Enemy_Bullet_Template::Update()
{
	UpdateFunction(this);
}