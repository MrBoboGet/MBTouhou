#pragma once
#include <Gameobject.h>
#include <Engine\Engine_GenericComponent.h>
#include <Engine.h>
//bas klass för fiender
class Enemy : public GameObject
{
private:

public:
	float MaxHp = 0;

	void DrawHealthbar();
	void DrawHealthbar(float Offset);
	Enemy(std::string Texture, float Size);
};
#include<Level_1_Enemy_2.h>
class Player : public GameObject
{
private:
	void Player_Teleport();
	//just nu med så få variablar så är det inte egentligen några problem, men i framtiden vill vi nog se till att varje funktion är ett objekt
public:
	float MaxEnergy = 100;
	float CurrentEnergy = 100;
	float EnergyRegenPerFrame = 1;
	float AttackEnergyCost = 0.5;
	float TeleportEnergyConsumption = 50;
	float TeleportDistance = 1;
	float TeleportBulletDestructionRadius = 1;
	bool KlickadeSpaceInnan = false;

	bool Key_Right = false;
	bool Key_Left = false;
	bool Key_Up = false;
	bool Key_Down = false;
	bool Key_Z = false;
	bool Key_X_Pressed = false;
	bool Key_Shift = false;
	bool Key_Space = false;
	bool Key_C = false;
	float Y_Change = 0;
	float X_Change = 0;

	int Bombs = 3;
	bool Key_X_Previous = false;
	bool GotHit = 0;
	int Invincible_Timer = 0;
	
	//ljudför spelaren
	TouhouSoundEngineSoundObject BasicShotSound = TouhouSoundEngineSoundObject("Resources/Sounds/PlayerBasicShot.wav","SoundEffect");
	TouhouSoundEngineSoundObject BombSound = TouhouSoundEngineSoundObject("Resources/Sounds/PlayerBomb.wav","SoundEffect");
	//Vi skulle kunna ha en generell getcomponent funktion som hittar componenten pga dess namn, och vi kan ha den i ett dictionary så att det är minns effektivt, samt en get component som gör detta
	std::vector<std::string> RequiredResources();
	Player(Vector2D Plats,std::string Namn,std::string Tagg);
	~Player();	
	float speed;
	virtual void Update();
};
class Level : public GameObject
{
public:
	float X_Limit = 4;
};
class Level1 : public Level
{
private:
	int Wave = 0;
	bool WaveCreated = false;
	bool EndBossSpawned = false;
	int EndBossSpawnTimer = 120;
	int WinDelay = 120;
	bool LevelISFinised = false;
public:
	//här lägger vi in lite information om banan, som dess begränsningar grafik etc. Vi börjar med den horisontella kanten
	//vi gör det till en variabel här, kan behöva bli 2 separata, men nu gör vi det enkelt och symmetriskt. Limiten räknas så att det är x steg från 
	//kanterna av skörmen
	virtual void Update();
	//eftersom levelns enda uppgift är att spawna in alla föremål som utgör leveln så gör vi så gör konstruktorn inget annat än att skapa den
	Level1();
	int Level1_Timer = 0;
	float Level_1_BGSpeed = 0.16f;
	float Level_1_BGPosition = 0;
	~Level1();
};
class Level1_Enemy_1 :public Enemy
{
public:
	Level1_Enemy_1(Vector2D Plats, std::string Namn, std::string Tagg);
	~Level1_Enemy_1();
	int Level1_Enemy_1_Timer = 0;
	float speed;
	virtual void Update();
};
class Enemy_Bullet : public GameObject
{
public:
	float Speed;
	//Vi kan undvika att behöva använda trigonometri genom att säga att vi har en kateter på x axeln som representerar ett tal från
	//1 till -1, som är katetern på enhets cirkeln, och att y sedan bara har ett värde som antingen är 1 eller -1 som visar vilket håll den ska åka
	//men jag tror när jag tänker efter att bara räkna med vinklar är bättre och mer futureproof
	//float X_Kateter;
	//float Y_Direction;
	float Direction;
	virtual void Update();
	Enemy_Bullet(Vector2D Plats, std::string Namn, std::string Tagg);
	~Enemy_Bullet();
};

//som det är nu är playerbullet tänkt att vara 1 kula, men ju mer jag tänker på det desto mer vill jag ha en klass som vi kan han för en all porpuse kula grej. Jag lägger till en constructor som låter oss ändra saker lite bättre
class Player_Bullet : public GameObject
{
public:
	float Speed;
	float Direction;
	float Damage = 1;

	virtual void Update();
	Player_Bullet(Vector2D Plats, std::string Namn, std::string Tagg);
	Player_Bullet(Vector2D Plats,std::string Texture, float Size);
	~Player_Bullet();
};