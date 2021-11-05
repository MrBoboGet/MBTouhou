#pragma once
#include <Gameobject.h>
#include <Engine\Engine_GenericComponent.h>
#include <Engine.h>
//bas klass f�r fiender
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
	//just nu med s� f� variablar s� �r det inte egentligen n�gra problem, men i framtiden vill vi nog se till att varje funktion �r ett objekt
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
	
	//ljudf�r spelaren
	TouhouSoundEngineSoundObject BasicShotSound = TouhouSoundEngineSoundObject("Resources/Sounds/PlayerBasicShot.wav","SoundEffect");
	TouhouSoundEngineSoundObject BombSound = TouhouSoundEngineSoundObject("Resources/Sounds/PlayerBomb.wav","SoundEffect");
	//Vi skulle kunna ha en generell getcomponent funktion som hittar componenten pga dess namn, och vi kan ha den i ett dictionary s� att det �r minns effektivt, samt en get component som g�r detta
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
	//h�r l�gger vi in lite information om banan, som dess begr�nsningar grafik etc. Vi b�rjar med den horisontella kanten
	//vi g�r det till en variabel h�r, kan beh�va bli 2 separata, men nu g�r vi det enkelt och symmetriskt. Limiten r�knas s� att det �r x steg fr�n 
	//kanterna av sk�rmen
	virtual void Update();
	//eftersom levelns enda uppgift �r att spawna in alla f�rem�l som utg�r leveln s� g�r vi s� g�r konstruktorn inget annat �n att skapa den
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
	//Vi kan undvika att beh�va anv�nda trigonometri genom att s�ga att vi har en kateter p� x axeln som representerar ett tal fr�n
	//1 till -1, som �r katetern p� enhets cirkeln, och att y sedan bara har ett v�rde som antingen �r 1 eller -1 som visar vilket h�ll den ska �ka
	//men jag tror n�r jag t�nker efter att bara r�kna med vinklar �r b�ttre och mer futureproof
	//float X_Kateter;
	//float Y_Direction;
	float Direction;
	virtual void Update();
	Enemy_Bullet(Vector2D Plats, std::string Namn, std::string Tagg);
	~Enemy_Bullet();
};

//som det �r nu �r playerbullet t�nkt att vara 1 kula, men ju mer jag t�nker p� det desto mer vill jag ha en klass som vi kan han f�r en all porpuse kula grej. Jag l�gger till en constructor som l�ter oss �ndra saker lite b�ttre
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