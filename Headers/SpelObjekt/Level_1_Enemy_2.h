#pragma once
#include<Enemy_Bullet_Template.h>
#include<SpelObjekt.h>

class Level_1_Enemy_2 : public Enemy
{
private:
	std::string m_TextureName = "";
public:
	Level_1_Enemy_2(Vector2D Plats, std::string Namn, std::string Tagg);
	~Level_1_Enemy_2();
	int Level_1_Enemy_2_Timer = 0;
	float speed;
	void Update() override;
	void OnCreate() override;
};
void Enemy_2_Kul_Logik(Enemy_Bullet_Template* a);