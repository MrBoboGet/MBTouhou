#pragma once
#include <Gameobject.h>
#include<SpelObjekt.h>
class Level_1_Enemy_3 : public Enemy
{
public:
	float Direction = 270;
	float Speed = 0.2;
	float ChargeTimeInFrames = 120;
	float BulletSpeed = 0.1f;
	float DistanceFromTopToStop = 2;

	int Timer = 0;

	void Update() override;
	Level_1_Enemy_3(Vector2D Position);
	~Level_1_Enemy_3();

private:

};
