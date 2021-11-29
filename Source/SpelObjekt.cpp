#include <SpelObjekt.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <MinMath.h>
#include <iostream>
#include <Hitbox.h>
#include <Player_Attack_BigShot.h>
#include <Level_1_Enemy_3.h>
#include <Engine.h>
#include <Engine\TextRendering.h>
#include<Engine\SpriteAnimationRenderer.h>
#include <Johan.h>
#include <PausMenu.h>
#include <MainMenu.h>
Player_Bullet::Player_Bullet(Vector2D Plats, std::string Namn, std::string Tagg) : GameObject("PlayerRegularBullet.png", 0.32)
{
	Position = Plats;
	Name = Namn;
	Tag = Tagg;
	//v�ldigt provosoriska siffror
	//vi g�r hitboxen st�rre �n spriten
	Hitbox = Vector2D(0.2, 0.2);
	Speed = 0.16f;
	Direction = 90;
}
Player_Bullet::Player_Bullet(Vector2D Plats, std::string Texture, float Size) : GameObject(Texture,Size)
{
	 //namn och tag tar vi som standard, man borde aktivt �ndra ist�llet
	Tag = "Player_Bullet";
	Name = "Player_Bullet";
	Position = Plats;
	//saker vi beh�ver faktiskt initializa	
	//Hitbox
	//Speed 
	//direction
	//vi s�tter dem till bullshit nu
	Hitbox = Vector2D(0, 0);
	Speed = 0;
	Direction = 90;
}
void Player_Bullet::Update()
{
	float X_Change = 0;
	float Y_Change = 0;
	X_Change += std::cos(MBMath::DegreeToRadian(Direction)) * Speed;
	Y_Change += std::sin(MBMath::DegreeToRadian(Direction)) * Speed;

	Position.x += X_Change;
	Position.y += Y_Change;

	int PositionIListan = 0;
	int Collision = 0;
	for (int i = 0; i < TouhouEngine::ActiveGameObjects.size(); i++)
	{
		if (TouhouEngine::ActiveGameObjects[i]->GetTag() == "Enemy")
		{
			if (CollisionClass::Rectangle_Collision(Position,Hitbox,Rotation,TouhouEngine::ActiveGameObjects[i]->Position, TouhouEngine::ActiveGameObjects[i]->Hitbox, TouhouEngine::ActiveGameObjects[i]->Rotation))
			{
				//nu vet vi att vi har kolliderat med ett f�rem�l med tagen fiende
				//det finns 2 s�tt att g�ra det h�r, 1, l�ta koden som skadar fienden vara i den eller l�ta den vara h�r
				//jag l�ter den vara h�r eftersom det blir enklare att h�lla koll p� och mindre copy paste
				TouhouEngine::ActiveGameObjects[i]->HP -= Damage;
				//borde vara baserat p� en damage variabel
				Collision = 1;
				for (int i = 0;i < TouhouEngine::ActiveGameObjects.size(); i++)
				{
					if (TouhouEngine::ActiveGameObjects[i] == this)
					{
						PositionIListan = i;
						break;
					}
				}
				break;
			}
		}
	}
	if (Collision == 1)
	{
		TouhouEngine::Destroy(this);
	}

}
Player_Bullet::~Player_Bullet()
{

}

//Eftersom vi bara kommer ha en spelare oavsett s� kommer vi att kunna ha variabler som den anv�nder h�r utan problem
int Player_Shot_Timer = 0;
int Player_Shot_Timer_Max = 10;
void Player::Update()
{
	//Rotation += 1;
	//std::cout << "vi kommer till update koden" << std::endl;
	Key_Right = false;
	Key_Left = false;
	Key_Up = false;
	Key_Down = false;
	Key_Z = false;
	Key_X_Pressed = false;
	Key_Shift = false;
	Key_Space = false;
	Key_C = false;
	Y_Change = 0;
	X_Change = 0;
	float Speed_Koeffecient = 1;
	//kollar alla keys
	if (true)
	{
		if (TouhouEngine::GetKeyDown("right"))
		{
			Key_Right = true;
		}
		if (TouhouEngine::GetKeyDown("left"))
		{
			Key_Left = true;
		}
		if (TouhouEngine::GetKeyDown("up"))
		{
			Key_Up = true;
		}
		if (TouhouEngine::GetKeyDown("down"))
		{
			Key_Down = true;
		}
		if (TouhouEngine::GetKeyDown("z"))
		{
			Key_Z = true;
		}
		if (TouhouEngine::GetKeyDown("leftshift"))
		{
			Key_Shift = true;
		}
		if (TouhouEngine::GetKeyDown("space"))
		{
			Key_Space = true;
		}
		if (TouhouEngine::GetKeyDown("c"))
		{
			Key_C = true;
		}
		//Eftersom vi vill se om det �r pressed s� vill vi g�ra att den �r bara sann n�r man klickade p� den
		if (TouhouEngine::GetKeyPressed("x"))
		{
			Key_X_Pressed = true;
		}
	}
	//s�tter v�rdet av X_Chnage beroende p� vilka keys som �r klickade
	if (true)
	{
		if (Key_Down)
		{
			Y_Change -= speed;
		}
		if (Key_Up)
		{
			Y_Change += speed;
		}
		if (Key_Right)
		{
			X_Change += speed;
		}
		if (Key_Left)
		{
			X_Change -= speed;
		}
	}
	//vi f�rs�ker refactora v�r kod och l�gger till mer struktur, jag g�r nu en separat funktion f�r teleport kod s� vi kan f�r lite mer struktur
	//Player_Teleport();
	//std::cout << X_Change << " " << Y_Change << std::endl;
	if (X_Change !=0 && Y_Change!=0)
	{
		X_Change /= std::sqrt(2);
		Y_Change /= std::sqrt(2);
	}
	//H�r k�r vi koden som g�r att spelaren kan skjuta
	if(Key_Z)
	{
		Player_Shot_Timer += 1;
		//g�r s� att energin minskar ocks� med samma speed som den reggar
		CurrentEnergy -= AttackEnergyCost;
	}
	if (Player_Shot_Timer == Player_Shot_Timer_Max)
	{
		//N�r timer �r lika med s� skapar vi en spelar kula
		GameObject* SpelarKula;
		GameObject* SpelarKula2;
		if (Key_Shift == false)
		{
			SpelarKula = new Player_Bullet(Vector2D(Position.x - 0.3f, Position.y), "Player_Bullet", "Player_Bullet");
			SpelarKula2 = new Player_Bullet(Vector2D(Position.x + 0.3f, Position.y), "Player_Bullet", "Player_Bullet");
		}
		else
		{
			SpelarKula = new Player_Bullet(Vector2D(Position.x - 0.15f, Position.y), "Player_Bullet", "Player_Bullet");
			SpelarKula2 = new Player_Bullet(Vector2D(Position.x + 0.15f, Position.y), "Player_Bullet", "Player_Bullet");
		}

		TouhouEngine::Create(SpelarKula);
		TouhouEngine::Create(SpelarKula2);
		TouhouEngine::PlaySound("Resources/Sounds/PlayerBasicShot.wav",0.5,"SoundEffect");
		//BasicShotSound.PlaySound();
		Player_Shot_Timer = 0;
	}
	//Grafik grejer

	//h�r ritar vi hur mycket HP man har
	for (int i = 0; i < HP; i++)
	{
		int Layer[] = { 100,1,0,0 };
		Texture::DrawTexture("Jakob.png", Vector2D(5 + i * 1.2f, -3), 1, 1,Layer);
	}
	for (int i = 0; i < Bombs; i++)
	{
		int Layer[] = { 100,1,0,0 };
		Texture::DrawTexture("Bomb.png", Vector2D(5 + i * 1.2f, -1.5), 1, 1, Layer);
	}
	//h�r k�r vi koden som styr vad som h�nder n�r spelaren blir tr�ffad
	if (Invincible_Timer == 0 && GotHit == 1)
	{
		HP -= 1;
		TouhouEngine::PlaySound("Resources/Sounds/Oof.wav", 0.5, "SoundEffect");
		Invincible_Timer = 60;
	}
	GotHit = 0;
	Invincible_Timer -= 1;
	if (Invincible_Timer < 0)
	{
		Invincible_Timer = 0;
	}
	if (Invincible_Timer > 0)
	{
		this->Renderer.ColorKoef.A = std::abs(std::sin(MBMath::DegreeToRadian(Invincible_Timer * 6))) * 0.5 + 0.3;
	}
	else
	{
		this->Renderer.ColorKoef.A = 1;
	}

	//nu �ndrar vi v�rdet av v�ran position sett till v�rdet utav X_Change och Y_Change
	//std::cout<< Position.x << " " << Position.y << " " << X_Change << " " << Y_Change << std::endl;
	
	//energi grejer
	if (CurrentEnergy < MaxEnergy)
	{
		CurrentEnergy += EnergyRegenPerFrame;
		if (CurrentEnergy > MaxEnergy)
		{
			CurrentEnergy = MaxEnergy;
		}
	}
	//nu ska vi �ven rita v�r fina energi m�tare
	int VisualLayerEnergy[] = { 100,1,0,0 };
	float FullLength = 4;
	Vector2D EnergybarPosition = Vector2D(-6, 0 -(FullLength - FullLength * (CurrentEnergy / MaxEnergy)/2));
	Texture::DrawTexture("Bluesquare.png", EnergybarPosition, 1, FullLength* (CurrentEnergy / MaxEnergy), VisualLayerEnergy);

	//Bomb koden
	std::vector<GameObject*> KulorAttDeleta = {};
	if (Bombs > 0 && Key_X_Pressed)
	{
		Bombs -= 1;
		//spela ljudet
		BombSound.PlaySound(0.1);
		for (int i = 0; i < TouhouEngine::ActiveGameObjects.size(); i++)
		{
			//std::cout << ActiveGameObjects[i]->GetTag() << std::endl;
			if (TouhouEngine::ActiveGameObjects[i]->GetTag() == "Enemy_Bullet")
			{
				KulorAttDeleta.push_back(TouhouEngine::ActiveGameObjects[i]);
			}
		}
	}
	for (int i = 0; i < KulorAttDeleta.size(); i++)
	{
		TouhouEngine::Destroy(KulorAttDeleta[i]);
	}
	//teleport kod
	Player_Teleport();
	//nu k�r vi v�ra komponent koder
	UpdateComponents();
	//om vi h�ller shift s� �ndrar vi speeden med en konstant koeffecient

	if (Key_Shift)
	{
		Speed_Koeffecient *= 0.5;
	}
	Position.x += X_Change*Speed_Koeffecient;
	Position.y += Y_Change*Speed_Koeffecient;
	//h�r ser vi till att spelaren �r inom banans gr�nser
	if (Position.y > 4.5)
	{
		Position.y = 4.5;
	}
	if (Position.y <-4.5)
	{
		Position.y = -4.5;
	}
	//nu beh�ver vi hitta banan variabeln
	int Level_Position;
	for (int i = 0; i < TouhouEngine::ActiveGameObjects.size();i++)
	{
		if (TouhouEngine::ActiveGameObjects[i]->GetTag() == "Level")
		{
			Level_Position = i;
			break;
		}
	}
	ASSERT(Level_Position >= 0);
	void* Leveln = TouhouEngine::ActiveGameObjects[Level_Position];
	Level* Leveln_Object{ static_cast<Level*>(Leveln)};
	if (Position.x > (8-Leveln_Object->X_Limit))
	{
		Position.x = (8 - Leveln_Object->X_Limit);
	}
	if (Position.x < (-8 + Leveln_Object->X_Limit))
	{
		Position.x = (-8 + Leveln_Object->X_Limit);
	}
	//Player* Player_Pointer{ static_cast<Player*>(Player_Pointer_Void) };

	//vi k�r koden f�r att rita grejewn efter all movmenet �r avklarad
	if (Key_Shift)
	{
		int VisualLayer[] = { 0,1,0,0 };
		Texture::DrawTexture("RedSquare.png", Position, Hitbox.x, Hitbox.y, VisualLayer);
	}
	//kod f�r att skrigva spelarens namn, bara f�r att testa
	std::vector<int> LayerF�rNamnet = { 10000000,1,1,1 };
	//DrawTextRectangle("Player Is Cool", Vector2D(-7,0), Vector2D(-3, -2), LayerF�rNamnet);

	//H�r k�r vi koden om vad som h�nder om HP �r mindre �n eller lika med 0
	//borde vara l�ngst ner f�r att vara s�ker p� att det �r det sista som h�nder
	if (HP <= 0)
	{
		//TouhouEngine::PlaySound("Resources/Sounds/OMyGodNejJohan.wav", 0.5);
		TouhouEngine::Destroy(this);
		if (TouhouEngine::FindObjectWithName("Johan") != nullptr)
		{
			TouhouEngine::PlaySound("Resources/Sounds/OMyGodNejJohan.wav", 0.5);
		}
	}
}
//vi hardcoadar vilken sprite och vilken storlek f�rem�let har h�r
Player::Player(Vector2D Plats, std::string Namn, std::string Tagg) : GameObject("Jakob.png",0.4)
{
	Position = Plats;
	Name = Namn;
	Rotation = 0;
	HP = 3;
	Bombs = 3;
	Tag = Tagg;
	//v�ldigt provosiriska siffror
	Hitbox = Vector2D(0.15f, 0.15f);
	speed = 0.1f;
	AddComponent(new Player_Attack_BigShot(this));
	//AddComponent(new SpriteAnimationRenderer(this, "PlayerAnimationConfig.txt"));
}
std::vector<std::string> Player::RequiredResources()
{
	std::vector<std::string> ReturV�rdet = std::vector<std::string>(0);
	ReturV�rdet.push_back("Resources/SpelResurser/Sprites/Jakob.png");
	ReturV�rdet.push_back("Resources/SpelResurser/Sprites/reverse.png");
	ReturV�rdet.push_back("Resources/Sounds/PlayerBomb.wav");
	ReturV�rdet.push_back("Resources/Sounds/PlayerBasicShot.wav");
	return(ReturV�rdet);
}
Player::~Player()
{

}
//spelaren
//Kul class
Enemy_Bullet::Enemy_Bullet(Vector2D Plats, std::string Namn, std::string Tagg) : GameObject("fiendeattack1.png", 0.16)
{
	Position = Plats;
	Name = Namn;
	Tag = Tagg;
	//v�ldigt provosoriska siffror
	Hitbox = Vector2D(0.16, .16);
	Speed = 0.08f;
	Direction = 270;
}
Enemy_Bullet::~Enemy_Bullet()
{

}
void Enemy_Bullet::Update()
{
	//den h�r koden g�r s� att kulan r�r sig �t det h�ll som directionen s�ger.
	//direction �r en vinkel fr�n 0-360 och man r�knar utifr�n den f�rsta kvadranten till den sista

	//vi s�tter direction s� att den �r likt rotationen
	Direction = Rotation + 270;

	float X_Change = 0;
	float Y_Change = 0;
	//beronde p� vinkel s� �ndrar vi nudessa v�rden
	X_Change += std::cos(MBMath::DegreeToRadian(Direction)) * Speed;
	Y_Change += std::sin(MBMath::DegreeToRadian(Direction)) * Speed;

	//std::cout << Position.x << " " << Position.y << " " << Math::Sin(270);
	//std::cout << "Kula existerar" << std::endl;
	//H�r �ndrar vi positionen s� att kulan faktiskt �ker
	Position.x += X_Change;
	Position.y += Y_Change;

	//vi k�r all kollision mellan spelare och kulor i kul koden, f�r den beh�ver bara hitta spelaren s� det blir snabbare
	//kollision kod


	//int PlayerObjectPosition = 0;
	//for (int i = 0; i < TouhouEngine::ActiveGameObjects.size(); i++)
	//{
	//	if(TouhouEngine::ActiveGameObjects[i]->GetTag() == "Player")
	//	{
	//		PlayerObjectPosition = i;
	//		break;
	//	}
	//}
	auto PlayerObject = TouhouEngine::FindObjectWithTag("Player");
	if (PlayerObject == nullptr)
	{
		return;
	}
	//Eftersom vi antagit att kvadratens sidor �r parallella med sk�rmens sidor beh�ver vi bara j�mf�ra om i tv� omg�ngar om
	//dem tv� olikheterna f�r x och y led var f�r sig kan ha gemensamma element, kan dem det s� �r det en kollision
	//Vi kollar om spelarens v�nstraste position �r mindre �n kulans h�graste och om spelarens h�grasta �r st�rre om kulans v�nstraste,
	//om det inte st�mmer s� kan det inte vara en kollision
	if(CollisionClass::Rectangle_Collision(Position,Hitbox,Rotation,PlayerObject->Position,PlayerObject->Hitbox,PlayerObject->Rotation))
	{
		//nu vet vi att en kollision h�nt
		//i v�rt fall just nu vill vi bara f�rst�ra kulan

		//ett problem vi f�r nu �r att vi inte riktigt vet var n�gonstans i listan av alla aktiva gameobject v�rt f�rem�l �r
		//beroende p� kan det ju vara s� att this pointer korresponderar med ett f�rem�l i listan.
		//std::cout << "Kollision har h�nt!" <<" "<<"Spelarens position: "<<PlayerObject->Position.x<<" "<<PlayerObject->Position.y<<" Kulan: "<<Position.x<<" "<<Position.y<< std::endl;
		//vi testar att loopa igenom alla pointers och ser om n�gon matchar this pointer

		//Nu n�r vi vet att en kollision h�nt s� vill vi att spelaren ska ta skada etc
		//H�r vill jag faktiskt att denna kod ska h�nda i spelaren av 2 anledningar
		//1. Om spelaren blir tr�ffad av 2 kulor samtidigt ska den inte ta 2 skada
		//2. den exakta proceduren kan �ndras och det blir enklare att �ndra i spelaren ist�llet f�r h�r
		//nu kommer vi till "spelaren �r tr�ffad flaggen"
		//Vi vill helst inte beh�va ha massa variablar i Gameobject koden, s� fr�gan �r hur vi f�r det 
		//den metod jag testar nu, �r att jag givet en pointer till en gameobject assignar en ny Player pointer s� jag kommer �t allt
		void* Player_Pointer_Void = PlayerObject;
		Player* Player_Pointer{static_cast<Player*>(Player_Pointer_Void)};
		Player_Pointer->GotHit = 1;
		TouhouEngine::Destroy(this);
	}
}
//Kul klass
//FIende 1 level 1
Level1_Enemy_1::Level1_Enemy_1(Vector2D Plats, std::string Namn, std::string Tagg) : Enemy("Fiende1.png", 0.8)
{
	Position = Plats;
	HP = 10;
	Hitbox = Vector2D(0.8, 0.8);
	Name = Namn;
	Tag = Tagg;
	speed = 0.01f;
	MaxHp = 10;
}
Level1_Enemy_1::~Level1_Enemy_1()
{

}
void Level1_Enemy_1::Update()
{
	//Ett intressant problem vi f�r med denna Update struktur �r att alla variablar som ska vara ett tag inte kan skapas h�r, utan m�ste
	//skaps i constructorn och med det �ven vara en del av spelobjektet, vilket verkligen inte �r optimalt
	//problemt vi f�r h�r �r ju hur vi ska g�ra en timer

	Position.y -= speed;

	//nu vill jag testa att skapa ett gameobject inom denna fiende. Tanken �r att vi dynamiskt tilldelar den minne med new, och sedan l�gger in
	//denna pointer i spelobjekt listan av f�rem�l. Tanken �r att eftersom en dynamiskt tilldelad variabel alltid kommer att vara i minnet tills
	//vi deletar den s� kommer pointer vi l�gger in inte "g� ur scope" utan forts�tta funka
	Level1_Enemy_1_Timer += 1;
	//std::cout << Level1_Enemy_1_Timer << std::endl;
	if (Level1_Enemy_1_Timer == 60)
	{
		for (int i = 0; i < 8; i++)
		{
			GameObject* Kula = new Enemy_Bullet(Position, "Kula", "Enemy_Bullet");
			TouhouEngine::Create(Kula);
			Kula->Rotation = 45 * i;
			Level1_Enemy_1_Timer = 0;
		}
	}
	if (HP <= 0)
	{
		TouhouEngine::Destroy(this);
	}

	//dra liv kod
	DrawHealthbar();
}
//fiende 1 level 1
//Level1
Level1::Level1()
{
	Tag = "Level";
}
Level1::~Level1()
{
}
bool PlayerWasAlive = true;
void Level1::Update()
{
	if ((TouhouEngine::FindObjectWithName("Spelaren") != nullptr || Level1_Timer == 0) || LevelISFinised == true)
	{
		std::vector<int> Layer123 = { 1000000,0,0,0 };
		if (LevelISFinised == false)
		{
			Level1_Timer += 1;
			PlayerWasAlive = true;
			int Layer[] = { 100,0,0,0 };
			int Layer2[] = { -100,0,0,0 };
			Texture::DrawTexture("RedSquare.png", Vector2D(6, 0), 4, 9, Layer);
			Texture::DrawTexture("RedSquare.png", Vector2D(-6, 0), 4, 9, Layer);
			//vi drar ban texturen
			Texture::DrawTexture("Backgrund3.png", Vector2D(0, (-4.5 + 18) - Level_1_BGPosition), 16, 36, Layer2);
			//test
			Level_1_BGPosition += Level_1_BGSpeed;
			Level_1_BGPosition = std::fmod(Level_1_BGPosition, 27);
			if (Level1_Timer == 1)
			{
				//vi vill testa den nya fienden
				GameObject* Spelaren = new Player(Vector2D(0.0f, 0.0f), "Spelaren", "Player");
				//TouhouEngine::Create(new Johan(Vector2D(0, 6)));
				TouhouEngine::Create(Spelaren);
				TouhouEngine::Create(new PausMenu());
			}
			//return;
			///*
			//DrawTextRectangle()
			//std::vector<int> Layer123 = { 1000000,0,0,0 };
			DrawTextRectangle("Time " + std::to_string(Level1_Timer / 60), Vector2D(-8, 4), Vector2D(-3, 2), Layer123, 0.4);
		}
		if (Level1_Timer == 120)
		{
			GameObject* Fiende1 = new Level_1_Enemy_3(Vector2D(0.0f, 5));
			TouhouEngine::Create(Fiende1);
		}
		if (Level1_Timer == 320)
		{
			GameObject* Fiende1 = new Level1_Enemy_1(Vector2D(0.0f, 3.15f), "Fiende1", "Enemy");
			TouhouEngine::Create(Fiende1);
			GameObject* Fiende2 = new Level1_Enemy_1(Vector2D(-3.0f, 3.15f), "Fiende2", "Enemy");
			TouhouEngine::Create(Fiende2);
			GameObject* Fiende3 = new Level1_Enemy_1(Vector2D(3.0f, 3.15f), "Fiende3", "Enemy");
			TouhouEngine::Create(Fiende3);

		}
		if (Level1_Timer == 520)
		{
			GameObject* Fiendeny = new Level_1_Enemy_2(Vector2D(0, 4), "Fiende", "Enemy");
			TouhouEngine::Create(Fiendeny);
		}
		if (Level1_Timer == 720)
		{
			GameObject* Fiendeny = new Level_1_Enemy_2(Vector2D(-3, 4), "Fiende", "Enemy");
			TouhouEngine::Create(Fiendeny);
			GameObject* Fiendeny2 = new Level_1_Enemy_2(Vector2D(3, 4), "Fiende", "Enemy");
			TouhouEngine::Create(Fiendeny2);

			GameObject* Fiende1 = new Level1_Enemy_1(Vector2D(0.0f, 3.15f), "Fiende1", "Enemy");
			TouhouEngine::Create(Fiende1);
			GameObject* Fiende2 = new Level1_Enemy_1(Vector2D(-3.0f, 3.15f), "Fiende2", "Enemy");
			TouhouEngine::Create(Fiende2);
			GameObject* Fiende3 = new Level1_Enemy_1(Vector2D(3.0f, 3.15f), "Fiende3", "Enemy");
			TouhouEngine::Create(Fiende3);
		}
		if (Level1_Timer == 920)
		{
			//tanken med denna banna �r att vi vill ha en strategisk aspekt mellan att d�da snipersen och den stora bl� i mitten
			TouhouEngine::Create(new Level_1_Enemy_3(Vector2D(-3, 5)));
			TouhouEngine::Create(new Level_1_Enemy_3(Vector2D(0, 5)));
			TouhouEngine::Create(new Level_1_Enemy_3(Vector2D(3, 5)));
			//satmidigt har vi 2 stora bl�a som �ker ner
			TouhouEngine::Create(new Level_1_Enemy_2(Vector2D(-2, 5), "Enemy_2", "Enemy"));
			TouhouEngine::Create(new Level_1_Enemy_2(Vector2D(2, 5), "Enemy_2", "Enemy"));
		}
		if (Level1_Timer == 1120)
		{
			TouhouEngine::Create(new Level_1_Enemy_2(Vector2D(-3, 5), "Enemy_2", "Enemy"));
			TouhouEngine::Create(new Level_1_Enemy_2(Vector2D(-1, 5), "Enemy_2", "Enemy"));
			TouhouEngine::Create(new Level_1_Enemy_2(Vector2D(1, 5), "Enemy_2", "Enemy"));
			TouhouEngine::Create(new Level_1_Enemy_2(Vector2D(3, 5), "Enemy_2", "Enemy"));

			TouhouEngine::Create(new Level1_Enemy_1(Vector2D(-3, 5), "Enemy1", "Enemy"));
			TouhouEngine::Create(new Level1_Enemy_1(Vector2D(-2, 5), "Enemy1", "Enemy"));
			TouhouEngine::Create(new Level1_Enemy_1(Vector2D(-1, 5), "Enemy1", "Enemy"));
			TouhouEngine::Create(new Level1_Enemy_1(Vector2D(1, 5), "Enemy1", "Enemy"));
			TouhouEngine::Create(new Level1_Enemy_1(Vector2D(2, 5), "Enemy1", "Enemy"));
			TouhouEngine::Create(new Level1_Enemy_1(Vector2D(3, 5), "Enemy1", "Enemy"));
		}
		if (Level1_Timer > 1120)
		{
			if (EndBossSpawned == false)
			{
				int NumberOfActiveEnemies = 0;
				for (size_t i = 0; i < TouhouEngine::ActiveGameObjects.size(); i++)
				{
					if (TouhouEngine::ActiveGameObjects[i]->GetTag() == "Enemy")
					{
						NumberOfActiveEnemies += 1;
					}
				}
				if (NumberOfActiveEnemies == 0)
				{
					//ska �ka till han kommer till 3
					EndBossSpawnTimer -= 1;
					//spawna ossian bilden med varning
					if ((EndBossSpawnTimer / 20) % 2 == 1)
					{
						int Layern[] = { 10000,0,0,0 };
						Texture::DrawTexture("Ossian.png", Vector2D(-3, 2.7), 1, 1, Layern);
						Texture::DrawTexture("Ossian.png", Vector2D(3, 2.7), 1, 1, Layern);
						std::vector<int> Layer3 = { 10000,0,0,0 };
						DrawTextRectangle("Varning", Vector2D(0, 2.7),Layer3, 0.4);
					}
				}
				if (EndBossSpawnTimer == 0)
				{
					TouhouEngine::Create(new Johan(Vector2D(0, 6)));
					EndBossSpawned = true;
				}
			}
			else
			{
				bool JohanExists = false;
				for (size_t i = 0; i < TouhouEngine::ActiveGameObjects.size(); i++)
				{
					if (TouhouEngine::ActiveGameObjects[i]->GetName() == "Johan")
					{
						JohanExists = true;
						break;
					}
				}
				if (!JohanExists)
				{
					//k�r endgame kod
					WinDelay -= 1;
					if (WinDelay <= 0)
					{
						LevelISFinised = true;
						//f�rst�r alla object utom level
						std::vector<GameObject*> ObjectAttDelete = {};
						for (int i = 0; i < TouhouEngine::ActiveGameObjects.size(); i++)
						{
							if (TouhouEngine::ActiveGameObjects[i]->GetTag() != "Level")
							{
								ObjectAttDelete.push_back(TouhouEngine::ActiveGameObjects[i]);
							}
						}
						for (int i = 0; i < ObjectAttDelete.size(); i++)
						{
							TouhouEngine::Destroy(ObjectAttDelete[i]);
						}
						//dra grejer till sk�rmen
						DrawTextRectangle("Congratulations", Vector2D(0, 2.75), Layer123,0.6);
						DrawTextRectangle("Your time is "+std::to_string(Level1_Timer/60), Vector2D(0, 0), Layer123,0.4);
						DrawTextRectangle("Press Esc to return to main menu", Vector2D(0, -2), Layer123,0.25);
						int Layer3[] = { 0,0,0,0 };
						Texture::DrawTexture("Backgrund1.png", Vector2D(0, 0), 16, 9, Layer3);
						if (TouhouEngine::GetKeyDown("esc"))
						{
							TouhouEngine::Destroy(this);
							TouhouEngine::Create(new MainMenu());
						}
					}
				}
			}
		}
	}
	else
	{
		//delete alla nuvarande objekt och skriv en ny text som �r gameover
		if (PlayerWasAlive == true)
		{
			std::vector<GameObject*> ObjectAttDelete = {};
			for (int i = 0; i < TouhouEngine::ActiveGameObjects.size(); i++)
			{
				if (TouhouEngine::ActiveGameObjects[i]->GetTag() != "Level")
				{
					ObjectAttDelete.push_back(TouhouEngine::ActiveGameObjects[i]);
				}
			}
			for (int i = 0; i < ObjectAttDelete.size(); i++)
			{
				TouhouEngine::Destroy(ObjectAttDelete[i]);
			}
		}
		//vi ritar game over, och sedan "klicka enter f�r att forts�tta
		std::vector<int> Layern = { 1,1,1,1 };
		int Layern2[4] = { 0,1,1,1 };
		Texture::DrawTexture("Backgrund1.png", Vector2D(0, 0), 16, 9, Layern2);
		DrawTextRectangle("GAME OVER", Vector2D(0, 2.75), Layern,0.6);
		DrawTextRectangle("Press enter to retry", Vector2D(0, -0.75), Layern);
		PlayerWasAlive = false;
		//kollar enter f�r att se om vi ska restarta
		if (TouhouEngine::GetKeyDown("enter"))
		{
			TouhouEngine::Destroy(this);
			TouhouEngine::Create(new Level1());
		}
	}
	//*/
}
//Level1
//Enemy_Bullet
//Player_Bullet

//bass klassen f�r fiender
Enemy::Enemy(std::string Texture, float size) : GameObject(Texture,size)
{

}
void Enemy::DrawHealthbar()
{
	//vi drar det med en konstant
	//dra liv kod
	float LifeWidth = 0.5;
	float LifeHeight = 0.125f;
	int LifeLayer[] = { 0,0,0,0 };
	for (int i = 0; i < 4;i++)
	{
		LifeLayer[i] = this->Renderer.Layer[i];
	}
	LifeLayer[1] += 1;
	float HealtbarPercentage = HP / MaxHp;
	Texture::DrawTexture("Green.png", Vector2D(Position.x - (LifeWidth / 2 - (LifeWidth / 2) * HealtbarPercentage), Position.y + 0.5f), LifeWidth * HealtbarPercentage, LifeHeight, LifeLayer);
	Texture::DrawTexture("RedSquare.png", Vector2D(Position.x + (LifeWidth / 2 - (LifeWidth / 2) * (1 - HealtbarPercentage)), Position.y + 0.5f), LifeWidth * (1 - HealtbarPercentage), LifeHeight, LifeLayer);
}
void Enemy::DrawHealthbar(float Offset)
{
	//vi drar det med en konstant
	//dra liv kod
	float LifeWidth = 0.5;
	float LifeHeight = 0.125f;
	int LifeLayer[] = { 0,0,0,0 };
	for (int i = 0; i < 4;i++)
	{
		LifeLayer[i] = this->Renderer.Layer[i];
	}
	LifeLayer[1] += 1;

	float HealtbarPercentage = HP / MaxHp;
	Texture::DrawTexture("Green.png", Vector2D(Position.x - (LifeWidth / 2 - (LifeWidth / 2) * HealtbarPercentage), Position.y + 0.5f+Offset), LifeWidth * HealtbarPercentage, LifeHeight, LifeLayer);
	Texture::DrawTexture("RedSquare.png", Vector2D(Position.x + (LifeWidth / 2 - (LifeWidth / 2) * (1 - HealtbarPercentage)), Position.y + 0.5f+Offset), LifeWidth * (1 - HealtbarPercentage), LifeHeight, LifeLayer);
	//std::cout << MaxHp << std::endl;
}