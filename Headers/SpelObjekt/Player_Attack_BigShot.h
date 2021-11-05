#pragma once
#include <SpelObjekt.h>
#include <Engine\Engine_GenericComponent.h>
class Player_Attack_BigShot : public Component
{
private:
	Player* Spelaren;
public:
	int Timer = 0;
	int ShotDelay = 10;
	float EnergyCost = 50;
	Player_Attack_BigShot(Player* Spelaren);
	~Player_Attack_BigShot();
	void Update() override;
private:

};
