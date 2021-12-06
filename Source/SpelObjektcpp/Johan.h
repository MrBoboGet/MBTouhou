#pragma once
#include <Engine.h>
#include <random>
#include <MBTouhouTypes.h>
class Johan;

class JohanHP : public MBTouhouEnemy_HP
{
public:
	void Update() override;
};
class JohanAttack
{
public:
	bool Finished = false;
	Johan* Object = nullptr;
	virtual void Update();
	MBGameEngine::MBGameEngine& GetEngine();
	JohanAttack();
	~JohanAttack();
};
class JohanCircelAroundAttack : public JohanAttack
{
private:
	Vector2D CircleCenter = Vector2D(0, 2.7);
	float CircleRadius = 0.7;
	float CirclingAnglePerFrame = 3;
	int ShootAttackIntervall = 30;
	int ShootAttackIntervallSide = 40;
	int AttackLength = 600;
	//variablar att resetta
	float CurrentAngle = 0;
	bool ShouldBeginToFinish = false;
	bool BegunCircling = false;
	int Timer = 0;

public:
	JohanCircelAroundAttack(Johan* ObjectToAttachTo);
	~JohanCircelAroundAttack();
	virtual void Update();
private:

};
class JohanRightToLeftAttack : public JohanAttack
{
private:
	float Speed = 0.06;
	int Direction = 1;
	float MaxAngelDeviance = 20;
	float MaxSpeedDifferance = 0.03;
	float MaxYDifference = 1;
	int BulletSpawnIntevall = 12;
	int NumberOfBulletsToSpawn = 3;
	std::default_random_engine Generator;
	Vector2D BulletSpawnRange = Vector2D(-4, 4);
	float JohanRange = 3;
	int AttackLength = 600;
	//variablar att resetta
	int Timer = 0;
public:
	virtual void Update();
	JohanRightToLeftAttack(Johan* ObjectToAttachTo);
	~JohanRightToLeftAttack();
};
class JohanCirclingBulletAttack : public JohanAttack
{
private:
	int AttackDelay = 220;
	int AttackDelay2 = 70;
	int AttackDuration = 600;
	//sacker att resetta
	int Timer = 0;
public:
	virtual void Update();
	JohanCirclingBulletAttack(Johan* ObjectToAttachTo);
	~JohanCirclingBulletAttack();

private:

};
class Johan : public GameObject
{
private:
	MBGameEngine::ObjectReference<MBTouhouEnemy_HP> m_HPComponent;
public:
	Johan(Vector2D Position);
	~Johan();
	int CurrentAttackIndex = 0;
	int JohanStartDelay = 60;
	int DestroyTimer = 60;
	int AttackDelay = 40;
	int AttackDelayTimer = 0;
	std::vector<JohanAttack*> Attacker = std::vector<JohanAttack*>(0);
	int MaxHp = 600;
	bool StateStart = true;
	float SpeedBeginning = 0.01;
	virtual void Update() override;
	virtual void OnCreate() override;
};