#include<Enemy_Bullet_Template.h>
#include <Hitbox.h>
Enemy_Bullet_Template::~Enemy_Bullet_Template()
{

}
Enemy_Bullet_Template::Enemy_Bullet_Template(Vector2D Plats, std::string Namn, std::string Tagg, std::string Bild, float Storlek, Vector2D Hitplox, void(*Funktion)(Enemy_Bullet_Template*)) : GameObject(Bild,Storlek)
{
	//AddComponent(new Rectangle_Hitbox());
	Position = Plats;
	UpdateFunction = Funktion;
	SetName(Namn);
	SetTag(Tagg);
	GetComponent<Rectangle_Hitbox>()->Width =Hitplox.x;
	GetComponent<Rectangle_Hitbox>()->Height =Hitplox.y;
	//
}
void Enemy_Bullet_Template::OnCreate()
{

}
void Enemy_Bullet_Template::Update()
{
	UpdateFunction(this);
}