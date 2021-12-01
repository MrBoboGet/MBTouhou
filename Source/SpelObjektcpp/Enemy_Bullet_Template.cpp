#include<Enemy_Bullet_Template.h>

Enemy_Bullet_Template::~Enemy_Bullet_Template()
{

}
Enemy_Bullet_Template::Enemy_Bullet_Template(Vector2D Plats, std::string Namn, std::string Tagg, std::string Bild, float Storlek, Vector2D Hitplox, void(*Funktion)(Enemy_Bullet_Template*)) : GameObject(Bild,Storlek)
{
	Position = Plats;
	UpdateFunction = Funktion;
	SetName(Namn);
	SetTag(Tagg);
	Hitbox = Hitplox;
	//
}
void Enemy_Bullet_Template::Update()
{
	UpdateFunction(this);
}