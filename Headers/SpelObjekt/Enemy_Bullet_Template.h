#pragma once
#include <Vector2D.h>
#include <string>
#include<Gameobject.h>
class Enemy_Bullet_Template : public GameObject
{
private:
	std::string m_TextureName = "";
public:
	float Speed;
	void(*UpdateFunction)(Enemy_Bullet_Template*);
	float Direction;
	int Timer = 0;
	virtual void Update();
	virtual void OnCreate() override;
	Enemy_Bullet_Template(Vector2D Plats, std::string Namn, std::string Tagg,std::string Bild, float Storlek, Vector2D hitplox,void(*Update)(Enemy_Bullet_Template*));
	~Enemy_Bullet_Template();
};