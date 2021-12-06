#pragma once
#include <SpelObjekt.h>
class Player_Attack_BigShot : public Component
{
private:
	MBGameEngine::ObjectReference<Player> Spelaren;
public:
	int Timer = 0;
	int ShotDelay = 10;
	float EnergyCost = 50;
	Player_Attack_BigShot(MBGameEngine::ObjectReference<Player> Spelaren);
	~Player_Attack_BigShot();
	void Update() override;
private:

};
