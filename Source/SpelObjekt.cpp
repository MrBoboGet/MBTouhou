#include <SpelObjekt.h>
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
#include <assert.h>

Player_Bullet::Player_Bullet(Vector2D Plats, std::string Namn, std::string Tagg)// : GameObject("PlayerRegularBullet.png", 0.32)
{
	Transform.SetPosition(Plats);
	SetName(Namn);
	SetTag(Tagg);
	AddComponent(new Rectangle_Hitbox());
	AddComponent(new SpriteRenderer());
	AddComponent(new MBTouhou_Bullet_DeleteOffScreen());
	//väldigt provosoriska siffror
	//vi gör hitboxen större än spriten
	//Hitbox = Vector2D(0.2, 0.2);
	m_TextureName = "PlayerRegularBullet.png";
	GetComponent<SpriteRenderer>()->Width = 0.32;
	Speed = 0.16f;
	Direction = 90;
}
Player_Bullet::Player_Bullet(Vector2D Plats, std::string Texture, float Size)// : GameObject(Texture,Size)
{
	 //namn och tag tar vi som standard, man borde aktivt ändra istället
	SetTag("Player_Bullet");
	SetName("Player_Bullet");
	AddComponent(new SpriteRenderer());
	AddComponent(new Rectangle_Hitbox());
	AddComponent(new MBTouhou_Bullet_DeleteOffScreen());
	GetComponent<SpriteRenderer>()->Width = Size;
	Transform.SetPosition(Plats);
	m_TextureName = Texture;
	//saker vi behöver faktiskt initializa	
	//Hitbox
	//Speed 
	//direction
	//vi sätter dem till bullshit nu
	//Hitbox = Vector2D(0, 0);
	m_TextureName = Texture;
	Speed = 0;
	Direction = 90;
}
void Player_Bullet::OnCreate()
{
	GetComponent<Rectangle_Hitbox>()->Width = 0.2;
	GetComponent<Rectangle_Hitbox>()->Height = 0.2;
	MBGameEngine::ObjectReference<SpriteRenderer> Renderer = GetComponent<SpriteRenderer>();
	Renderer->SpriteTexture = GetEngine().LoadNamedTexture(m_TextureName, "Resources/SpelResurser/Sprites/" + m_TextureName);
}
void Player_Bullet::Update()
{
	float X_Change = 0;
	float Y_Change = 0;
	X_Change += std::cos(MBMath::DegreeToRadian(Direction)) * Speed;
	Y_Change += std::sin(MBMath::DegreeToRadian(Direction)) * Speed;

	//Position.x += X_Change;
	//Position.y += Y_Change;
	Transform.SetPosition(Vector2D(Transform.GetPosition()) + Vector2D(X_Change, Y_Change));

	int Collision = 0;
	ActiveObjectIterator ObjectIterator = GetEngine().GetActiveObjectsIterator();
	while (ObjectIterator.HasEnded() == false)
	{
		if (ObjectIterator->GetTag() == "Enemy")
		{
			if (Rectangle_Hitbox::Collides(GetComponent<Rectangle_Hitbox>(),ObjectIterator->GetComponent<Rectangle_Hitbox>()))
			{
				ObjectIterator->GetComponent<MBTouhouEnemy_HP>()->HP -= Damage;
				Collision = 1;
				break;
			}
		}
		ObjectIterator++;
	}
	if (Collision == 1)
	{
		GetEngine().Destroy(this);
	}
}
Player_Bullet::~Player_Bullet()
{

}

//Eftersom vi bara kommer ha en spelare oavsett så kommer vi att kunna ha variabler som den använder här utan problem
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


	Vector2D Position = Transform.GetPosition();
	//kollar alla keys
	if (true)
	{
		if (GetEngine().GetKeyDown(MBGameEngine::KeyCode::Right))
		{
			Key_Right = true;
		}
		if (GetEngine().GetKeyDown(MBGameEngine::KeyCode::Left))
		{
			Key_Left = true;
		}
		if (GetEngine().GetKeyDown(MBGameEngine::KeyCode::Up))
		{
			Key_Up = true;
		}
		if (GetEngine().GetKeyDown(MBGameEngine::KeyCode::Down))
		{
			Key_Down = true;
		}
		if (GetEngine().GetKeyDown(MBGameEngine::KeyCode::Z))
		{
			Key_Z = true;
		}
		if (GetEngine().GetKeyDown(MBGameEngine::KeyCode::LeftShift))
		{
			Key_Shift = true;
		}
		if (GetEngine().GetKeyDown(MBGameEngine::KeyCode::Space))
		{
			Key_Space = true;
		}
		if (GetEngine().GetKeyDown(MBGameEngine::KeyCode::C))
		{
			Key_C = true;
		}
		//Eftersom vi vill se om det är pressed så vill vi göra att den är bara sann när man klickade på den
		if (GetEngine().GetKeyPressed(MBGameEngine::KeyCode::X))
		{
			Key_X_Pressed = true;
		}
	}
	//sätter värdet av X_Chnage beroende på vilka keys som är klickade
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
	//vi försöker refactora vår kod och lägger till mer struktur, jag gör nu en separat funktion för teleport kod så vi kan får lite mer struktur
	//Player_Teleport();
	//std::cout << X_Change << " " << Y_Change << std::endl;
	if (X_Change !=0 && Y_Change!=0)
	{
		X_Change /= std::sqrt(2);
		Y_Change /= std::sqrt(2);
	}
	//Här kör vi koden som gör att spelaren kan skjuta
	if(Key_Z)
	{
		Player_Shot_Timer += 1;
		//gör så att energin minskar också med samma speed som den reggar
		CurrentEnergy -= AttackEnergyCost;
	}
	if (Player_Shot_Timer == Player_Shot_Timer_Max)
	{
		//När timer är lika med så skapar vi en spelar kula
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

		GetEngine().Create(SpelarKula);
		GetEngine().Create(SpelarKula2);
		GetEngine().PlaySound("Resources/Sounds/PlayerBasicShot.wav",0.5,"SoundEffect");
		//BasicShotSound.PlaySound();
		Player_Shot_Timer = 0;
	}
	//Grafik grejer

	//här ritar vi hur mycket HP man har
	for (int i = 0; i < Bombs; i++)
	{
		std::array<int,4> Layer = { 100,1,0,0 };
		GetEngine().DrawTexture("Bomb.png", Vector2D(5 + i * 1.2f, -1.5), 1, 1, Layer);
	}

	//nu ändrar vi värdet av våran position sett till värdet utav X_Change och Y_Change
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
	//nu ska vi även rita vår fina energi mätare
	std::array<int,4> VisualLayerEnergy = { 100,1,0,0 };
	float FullLength = 4;
	Vector2D EnergybarPosition = Vector2D(-6, 0 -(FullLength - FullLength * (CurrentEnergy / MaxEnergy)/2));
	GetEngine().DrawTexture("Bluesquare.png", EnergybarPosition, 1, FullLength* (CurrentEnergy / MaxEnergy), VisualLayerEnergy);

	//Bomb koden
	std::vector<MBGameEngine::GameObjectReference> KulorAttDeleta = {};
	if (Bombs > 0 && Key_X_Pressed)
	{
		Bombs -= 1;
		//spela ljudet
		BombSound.PlaySound(0.1);
		ActiveObjectIterator ObjectIterator = GetEngine().GetActiveObjectsIterator();
		while (ObjectIterator.HasEnded() == false)
		{
			if (ObjectIterator->GetTag() == "Enemy_Bullet")
			{
				KulorAttDeleta.push_back(*ObjectIterator);
			}
			ObjectIterator++;
		}
		//for (int i = 0; i < GetEngine().ActiveGameObjects.size(); i++)
		//{
		//	//std::cout << ActiveGameObjects[i]->GetTag() << std::endl;
		//	if (GetEngine().ActiveGameObjects[i]->GetTag() == "Enemy_Bullet")
		//	{
		//		KulorAttDeleta.push_back(GetEngine().ActiveGameObjects[i]);
		//	}
		//}
	}
	for (int i = 0; i < KulorAttDeleta.size(); i++)
	{
		GetEngine().Destroy(KulorAttDeleta[i]);
	}
	//teleport kod
	Player_Teleport();
	Position = Transform.GetPosition();
	//nu kör vi våra komponent koder
	
	
	//UpdateComponents();
	
	
	//om vi håller shift så ändrar vi speeden med en konstant koeffecient

	if (Key_Shift)
	{
		Speed_Koeffecient *= 0.5;
	}
	Position.x += X_Change*Speed_Koeffecient;
	Position.y += Y_Change*Speed_Koeffecient;
	//här ser vi till att spelaren är inom banans gränser
	if (Position.y > 4.5)
	{
		Position.y = 4.5;
	}
	if (Position.y <-4.5)
	{
		Position.y = -4.5;
	}
	//nu behöver vi hitta banan variabeln
	//int Level_Position;
	MBGameEngine::ObjectReference<Level> Leveln;
	ActiveObjectIterator ObjectIterator = GetEngine().GetActiveObjectsIterator();
	while (ObjectIterator.HasEnded() == false)
	{
		if (ObjectIterator->GetTag() == "Level")
		{
			Leveln = *ObjectIterator;
			break;
		}
		ObjectIterator++;
	}
	//for (int i = 0; i < GetEngine().ActiveGameObjects.size();i++)
	//{
	//	if (GetEngine().ActiveGameObjects[i]->GetTag() == "Level")
	//	{
	//		Level_Position = i;
	//		break;
	//	}
	//}
	assert(Leveln != nullptr);
	//void* Leveln = LevelPointer;
	if (Position.x > (8- Leveln->X_Limit))
	{
		Position.x = (8 - Leveln->X_Limit);
	}
	if (Position.x < (-8 + Leveln->X_Limit))
	{
		Position.x = (-8 + Leveln->X_Limit);
	}
	//Player* Player_Pointer{ static_cast<Player*>(Player_Pointer_Void) };

	//vi kör koden för att rita grejewn efter all movmenet är avklarad
	if (Key_Shift)
	{
		std::array<int,4> VisualLayer = { 0,1,0,0 };
		GetEngine().DrawTexture("RedSquare.png", Position, GetComponent<Rectangle_Hitbox>()->Width, GetComponent<Rectangle_Hitbox>()->Height, VisualLayer);
	}
	//kod för att skrigva spelarens namn, bara för att testa
	std::vector<int> LayerFörNamnet = { 10000000,1,1,1 };
	//DrawTextRectangle("Player Is Cool", Vector2D(-7,0), Vector2D(-3, -2), LayerFörNamnet);

	//Här kör vi koden om vad som händer om HP är mindre än eller lika med 0
	//borde vara längst ner för att vara säker på att det är det sista som händer
	Transform.SetPosition(Position);
}
//vi hardcoadar vilken sprite och vilken storlek föremålet har här
Player::Player(Vector2D Plats, std::string Namn, std::string Tagg)// : GameObject("Jakob.png",0.4)
{
	//Position = Plats;
	Transform.SetPosition(Plats);
	SetName(Namn);
	//Rotation = 0;
	//HP = 3;
	Bombs = 3;
	SetTag(Tagg);
	//väldigt provosiriska siffror
	//Hitbox = Vector2D(0.15f, 0.15f);
	speed = 0.1f;
	AddComponent(new SpriteRenderer());
	AddComponent(new Player_Attack_BigShot(GetReference()));
	AddComponent(new Rectangle_Hitbox());
	AddComponent(new TouhouPlayer_HP(3));
	//AddComponent(new SpriteAnimationRenderer(this, "PlayerAnimationConfig.txt"));
}
void Player::OnCreate()
{
	GetComponent<Rectangle_Hitbox>()->Width = 0.15;
	GetComponent<Rectangle_Hitbox>()->Height = 0.15;
	MBGameEngine::ObjectReference<SpriteRenderer> Renderer = GetComponent<SpriteRenderer>();
	Renderer->Width = 0.4;
	Renderer->SpriteTexture = GetEngine().LoadNamedTexture("Jakob.png", "Resources/SpelResurser/Sprites/Jakob.png");
}
std::vector<std::string> Player::RequiredResources()
{
	std::vector<std::string> ReturnValue = std::vector<std::string>(0);
	ReturnValue.push_back("Resources/SpelResurser/Sprites/Jakob.png");
	ReturnValue.push_back("Resources/SpelResurser/Sprites/reverse.png");
	ReturnValue.push_back("Resources/Sounds/PlayerBomb.wav");
	ReturnValue.push_back("Resources/Sounds/PlayerBasicShot.wav");
	return(ReturnValue);
}
Player::~Player()
{
	//std::cout << "Hmm" << std::endl;
}
//spelaren
//Kul class
Enemy_Bullet::Enemy_Bullet(Vector2D Plats, std::string Namn, std::string Tagg)// : GameObject("fiendeattack1.png", 0.16)
{
	Transform.SetPosition(Plats);
	SetName(Namn);
	SetTag(Tagg);
	//väldigt provosoriska siffror
	//Hitbox = Vector2D(0.16, .16);
	AddComponent(new SpriteRenderer());
	AddComponent(new Rectangle_Hitbox());
	AddComponent(new MBTouhou_Bullet_DeleteOffScreen());
	Speed = 0.08f;
	Direction = 270;
}
void Enemy_Bullet::OnCreate()
{
	GetComponent<Rectangle_Hitbox>()->Width = 0.16;
	GetComponent<Rectangle_Hitbox>()->Height = 0.16;
	MBGameEngine::ObjectReference<SpriteRenderer> Renderer = GetComponent<SpriteRenderer>();
	Renderer->Width = 0.16;
	Renderer->SpriteTexture = GetEngine().LoadNamedTexture("fiendeattack1.png", "Resources/SpelResurser/Sprites/fiendeattack1.png");
}
Enemy_Bullet::~Enemy_Bullet()
{

}
void Enemy_Bullet::Update()
{
	//den här koden gör så att kulan rör sig åt det håll som directionen säger.
	//direction är en vinkel från 0-360 och man räknar utifrån den första kvadranten till den sista
	Vector2D Position = Transform.GetPosition();
	//vi sätter direction så att den är likt rotationen
	Direction = Transform.GetRotation()[0] + 270;

	float X_Change = 0;
	float Y_Change = 0;
	//beronde på vinkel så ändrar vi nudessa värden
	X_Change += std::cos(MBMath::DegreeToRadian(Direction)) * Speed;
	Y_Change += std::sin(MBMath::DegreeToRadian(Direction)) * Speed;

	//AddComponent(new Rectangle_Hitbox());
	Position.x += X_Change;
	Position.y += Y_Change;
	auto PlayerObject = GetEngine().FindObjectWithTag("Player");
	if (PlayerObject == nullptr)
	{
		return;
	}
	if(Rectangle_Hitbox::Collides(GetComponent<Rectangle_Hitbox>(),PlayerObject->GetComponent<Rectangle_Hitbox>()))
	{
		PlayerObject->GetComponent<TouhouPlayer_HP>()->RegisterCollision();
		GetEngine().Destroy(this);
	}
	Transform.SetPosition(Position);
}
//Kul klass
//FIende 1 level 1
Level1_Enemy_1::Level1_Enemy_1(Vector2D Plats, std::string Namn, std::string Tagg)// : Enemy("Fiende1.png", 0.8)
{
	Transform.SetPosition(Plats);
	//HP = 10;
	GetComponent<MBTouhouEnemy_HP>()->HP = 10;
	GetComponent<MBTouhouEnemy_HP>()->MaxHP = 10;
	
	//Hitbox = Vector2D(0.8, 0.8);
	SetName(Namn);
	SetTag(Tagg);
	GetComponent<SpriteRenderer>()->Width = 0.8;
	m_TextureName = "Fiende1.png";
	speed = 0.01f;
	//MaxHp = 10;
}
void Level1_Enemy_1::OnCreate()
{
	GetComponent<Rectangle_Hitbox>()->Height = 0.8;
	GetComponent<Rectangle_Hitbox>()->Width = 0.8;
	GetComponent<SpriteRenderer>()->SpriteTexture = GetEngine().LoadNamedTexture("Fiende1.png", "Resources/SpelResurser/Sprites/Fiende1.png");
}
Level1_Enemy_1::~Level1_Enemy_1()
{

}
void Level1_Enemy_1::Update()
{
	//Ett intressant problem vi får med denna Update struktur är att alla variablar som ska vara ett tag inte kan skapas här, utan måste
	//skaps i constructorn och med det även vara en del av spelobjektet, vilket verkligen inte är optimalt
	//problemt vi får här är ju hur vi ska göra en timer

	Transform.SetPosition(Transform.GetPosition() - MBMath::MBVector3<float>(0,speed,0));

	//nu vill jag testa att skapa ett gameobject inom denna fiende. Tanken är att vi dynamiskt tilldelar den minne med new, och sedan lägger in
	//denna pointer i spelobjekt listan av föremäl. Tanken är att eftersom en dynamiskt tilldelad variabel alltid kommer att vara i minnet tills
	//vi deletar den så kommer pointer vi lägger in inte "gå ur scope" utan fortsätta funka
	Level1_Enemy_1_Timer += 1;
	//std::cout << Level1_Enemy_1_Timer << std::endl;
	if (Level1_Enemy_1_Timer == 60)
	{
		for (int i = 0; i < 8; i++)
		{
			GameObject* Kula = new Enemy_Bullet(Transform.GetPosition(), "Kula", "Enemy_Bullet");
			GetEngine().Create(Kula);
			Kula->Transform.SetRotation(MBMath::MBVector3<float>(45 * i,0,0));
			Level1_Enemy_1_Timer = 0;
		}
	}
	//if (HP <= 0)
	//{
	//	GetEngine().Destroy(this);
	//}

	//dra liv kod
	//DrawHealthbar();
}
//fiende 1 level 1
//Level1
Level1::Level1()
{
	SetTag("Level");
}
Level1::~Level1()
{
}
bool PlayerWasAlive = true;
void Level1::Update()
{
	if ((GetEngine().FindObjectWithName("Spelaren") != nullptr || Level1_Timer == 0) || LevelISFinised == true)
	{
		std::array<int,4> Layer123 = { 1000000,0,0,0 };
		if (LevelISFinised == false)
		{
			Level1_Timer += 1;
			PlayerWasAlive = true;
			std::array<int,4> Layer = { 100,0,0,0 };
			std::array<int,4> Layer2 = { -100,0,0,0 };
			GetEngine().DrawTexture("RedSquare.png", Vector2D(6, 0), 4, 9, Layer);
			GetEngine().DrawTexture("RedSquare.png", Vector2D(-6, 0), 4, 9, Layer);
			//vi drar ban texturen
			GetEngine().DrawTexture("Backgrund3.png", Vector2D(0, (-4.5 + 18) - Level_1_BGPosition), 16, 36, Layer2);
			//test
			Level_1_BGPosition += Level_1_BGSpeed;
			Level_1_BGPosition = std::fmod(Level_1_BGPosition, 27);
			if (Level1_Timer == 1)
			{
				//vi vill testa den nya fienden
				GameObject* Spelaren = new Player(Vector2D(0.0f, 0.0f), "Spelaren", "Player");
				//GetEngine().Create(new Johan(Vector2D(0, 6)));
				GetEngine().Create(Spelaren);
				GetEngine().Create(new PausMenu());
			}
			//return;
			///*
			//DrawTextRectangle()
			//std::vector<int> Layer123 = { 1000000,0,0,0 };
			DrawTextRectangle(GetEngine(), "Time " + std::to_string(Level1_Timer / 60), Vector2D(-8, 4), Vector2D(-3, 2), Layer123, 0.4);
		}
		if (Level1_Timer == 120)
		{
			GameObject* Fiende1 = new Level_1_Enemy_3(Vector2D(0.0f, 5));
			GetEngine().Create(Fiende1);
		}
		if (Level1_Timer == 320)
		{
			GameObject* Fiende1 = new Level1_Enemy_1(Vector2D(0.0f, 3.15f), "Fiende1", "Enemy");
			GetEngine().Create(Fiende1);
			GameObject* Fiende2 = new Level1_Enemy_1(Vector2D(-3.0f, 3.15f), "Fiende2", "Enemy");
			GetEngine().Create(Fiende2);
			GameObject* Fiende3 = new Level1_Enemy_1(Vector2D(3.0f, 3.15f), "Fiende3", "Enemy");
			GetEngine().Create(Fiende3);

		}
		if (Level1_Timer == 520)
		{
			GameObject* Fiendeny = new Level_1_Enemy_2(Vector2D(0, 4), "Fiende", "Enemy");
			GetEngine().Create(Fiendeny);
		}
		if (Level1_Timer == 720)
		{
			GameObject* Fiendeny = new Level_1_Enemy_2(Vector2D(-3, 4), "Fiende", "Enemy");
			GetEngine().Create(Fiendeny);
			GameObject* Fiendeny2 = new Level_1_Enemy_2(Vector2D(3, 4), "Fiende", "Enemy");
			GetEngine().Create(Fiendeny2);

			GameObject* Fiende1 = new Level1_Enemy_1(Vector2D(0.0f, 3.15f), "Fiende1", "Enemy");
			GetEngine().Create(Fiende1);
			GameObject* Fiende2 = new Level1_Enemy_1(Vector2D(-3.0f, 3.15f), "Fiende2", "Enemy");
			GetEngine().Create(Fiende2);
			GameObject* Fiende3 = new Level1_Enemy_1(Vector2D(3.0f, 3.15f), "Fiende3", "Enemy");
			GetEngine().Create(Fiende3);
		}
		if (Level1_Timer == 920)
		{
			//tanken med denna banna är att vi vill ha en strategisk aspekt mellan att döda snipersen och den stora blå i mitten
			GetEngine().Create(new Level_1_Enemy_3(Vector2D(-3, 5)));
			GetEngine().Create(new Level_1_Enemy_3(Vector2D(0, 5)));
			GetEngine().Create(new Level_1_Enemy_3(Vector2D(3, 5)));
			//satmidigt har vi 2 stora blåa som åker ner
			GetEngine().Create(new Level_1_Enemy_2(Vector2D(-2, 5), "Enemy_2", "Enemy"));
			GetEngine().Create(new Level_1_Enemy_2(Vector2D(2, 5), "Enemy_2", "Enemy"));
		}
		if (Level1_Timer == 1120)
		{
			GetEngine().Create(new Level_1_Enemy_2(Vector2D(-3, 5), "Enemy_2", "Enemy"));
			GetEngine().Create(new Level_1_Enemy_2(Vector2D(-1, 5), "Enemy_2", "Enemy"));
			GetEngine().Create(new Level_1_Enemy_2(Vector2D(1, 5), "Enemy_2", "Enemy"));
			GetEngine().Create(new Level_1_Enemy_2(Vector2D(3, 5), "Enemy_2", "Enemy"));

			GetEngine().Create(new Level1_Enemy_1(Vector2D(-3, 5), "Enemy1", "Enemy"));
			GetEngine().Create(new Level1_Enemy_1(Vector2D(-2, 5), "Enemy1", "Enemy"));
			GetEngine().Create(new Level1_Enemy_1(Vector2D(-1, 5), "Enemy1", "Enemy"));
			GetEngine().Create(new Level1_Enemy_1(Vector2D(1, 5), "Enemy1", "Enemy"));
			GetEngine().Create(new Level1_Enemy_1(Vector2D(2, 5), "Enemy1", "Enemy"));
			GetEngine().Create(new Level1_Enemy_1(Vector2D(3, 5), "Enemy1", "Enemy"));
		}
		if (Level1_Timer > 1120)
		{
			if (EndBossSpawned == false)
			{
				int NumberOfActiveEnemies = 0;
				ActiveObjectIterator ObjectIterator = GetEngine().GetActiveObjectsIterator();
				while(ObjectIterator.HasEnded() == false)
				{
					if (ObjectIterator->GetTag() == "Enemy")
					{
						NumberOfActiveEnemies += 1;
					}
					ObjectIterator++;
				}
				//for (size_t i = 0; i < GetEngine().ActiveGameObjects.size(); i++)
				//{
				//	if (GetEngine().ActiveGameObjects[i]->GetTag() == "Enemy")
				//	{
				//		NumberOfActiveEnemies += 1;
				//	}
				//}
				if (NumberOfActiveEnemies == 0)
				{
					//ska åka till han kommer till 3
					EndBossSpawnTimer -= 1;
					//spawna ossian bilden med varning
					if ((EndBossSpawnTimer / 20) % 2 == 1)
					{
						std::array<int,4> Layern = { 10000,0,0,0 };
						GetEngine().DrawTexture("Ossian.png", Vector2D(-3, 2.7), 1, 1, Layern);
						GetEngine().DrawTexture("Ossian.png", Vector2D(3, 2.7), 1, 1, Layern);
						std::array<int,4> Layer3 = { 10000,0,0,0 };
						DrawTextRectangle(GetEngine(), "Varning", Vector2D(0, 2.7), Layer3, 0.4);
					}
				}
				if (EndBossSpawnTimer == 0)
				{
					GetEngine().Create(new Johan(Vector2D(0, 6)));
					EndBossSpawned = true;
				}
			}
			else
			{
				bool JohanExists = false;
				ActiveObjectIterator ObjectIterator = GetEngine().GetActiveObjectsIterator();
				while (ObjectIterator.HasEnded() == false)
				{
					if (ObjectIterator->GetName() == "Johan")
					{
						JohanExists = true;
						break;
					}
					ObjectIterator++;
				}
				//for (size_t i = 0; i < GetEngine().ActiveGameObjects.size(); i++)
				//{
				//	if (GetEngine().ActiveGameObjects[i]->GetName() == "Johan")
				//	{
				//		JohanExists = true;
				//		break;
				//	}
				//}
				if (!JohanExists)
				{
					//kör endgame kod
					WinDelay -= 1;
					if (WinDelay <= 0)
					{
						LevelISFinised = true;
						//förstör alla object utom level
						std::vector <MBGameEngine::GameObjectReference> ObjectAttDelete = {};
						while (ObjectIterator.HasEnded() == false)
						{
							if (ObjectIterator->GetTag() != "Level")
							{
								ObjectAttDelete.push_back(*ObjectIterator);
							}
							ObjectIterator++;
						}
						//for (int i = 0; i < GetEngine().ActiveGameObjects.size(); i++)
						//{
						//	if (GetEngine().ActiveGameObjects[i]->GetTag() != "Level")
						//	{
						//		ObjectAttDelete.push_back(GetEngine().ActiveGameObjects[i]);
						//	}
						//}
						for (int i = 0; i < ObjectAttDelete.size(); i++)
						{
							GetEngine().Destroy(ObjectAttDelete[i]);
						}
						//dra grejer till skärmen
						DrawTextRectangle(GetEngine(),"Congratulations", Vector2D(0, 2.75), Layer123,0.6);
						DrawTextRectangle(GetEngine(),"Your time is "+std::to_string(Level1_Timer/60), Vector2D(0, 0), Layer123,0.4);
						DrawTextRectangle(GetEngine(),"Press Esc to return to main menu", Vector2D(0, -2), Layer123,0.25);
						std::array<int,4> Layer3 = { 0,0,0,0 };
						GetEngine().DrawTexture("Backgrund1.png", Vector2D(0, 0), 16, 9, Layer3);
						if (GetEngine().GetKeyDown(MBGameEngine::KeyCode::Esc))
						{
							GetEngine().Destroy(this);
							GetEngine().Create(new MainMenu());
						}
					}
				}
			}
		}
	}
	else
	{
		//delete alla nuvarande objekt och skriv en ny text som är gameover
		if (PlayerWasAlive == true)
		{
			std::vector < MBGameEngine::GameObjectReference > ObjectAttDelete = {};
			ActiveObjectIterator ObjectIterator = GetEngine().GetActiveObjectsIterator();
			
			while (ObjectIterator.HasEnded() == false)
			{
				if (ObjectIterator->GetTag() != "Level")
				{
					ObjectAttDelete.push_back(*ObjectIterator);
				}
				ObjectIterator++;
			}
			//for (int i = 0; i < GetEngine().ActiveGameObjects.size(); i++)
			//{
			//	if (GetEngine().ActiveGameObjects[i]->GetTag() != "Level")
			//	{
			//		ObjectAttDelete.push_back(GetEngine().ActiveGameObjects[i]);
			//	}
			//}
			for (int i = 0; i < ObjectAttDelete.size(); i++)
			{
				GetEngine().Destroy(ObjectAttDelete[i]);
			}
		}
		//vi ritar game over, och sedan "klicka enter för att fortsätta
		std::array<int,4> Layern = { 1,1,1,1 };
		std::array<int,4> Layern2 = { 0,1,1,1 };
		GetEngine().DrawTexture("Backgrund1.png", Vector2D(0, 0), 16, 9, Layern2);
		DrawTextRectangle(GetEngine(),"GAME OVER", Vector2D(0, 2.75), Layern,0.6);
		DrawTextRectangle(GetEngine(),"Press enter to retry", Vector2D(0, -0.75), Layern);
		PlayerWasAlive = false;
		//kollar enter för att se om vi ska restarta
		if (GetEngine().GetKeyDown(MBGameEngine::KeyCode::Enter))
		{
			GetEngine().Destroy(this);
			GetEngine().Create(new Level1());
		}
	}
	//*/
}
//Level1
//Enemy_Bullet
//Player_Bullet

//bass klassen för fiender
//Enemy::Enemy(std::string Texture, float size) : GameObject(Texture,size)
//{
//	AddComponent(new SpriteRenderer());
//	AddComponent(new Rectangle_Hitbox());
//}
//Enemy::Enemy()
//{
//	AddComponent(new SpriteRenderer());
//	AddComponent(new Rectangle_Hitbox());
//}
//void Enemy::DrawHealthbar()
//{
//	//vi drar det med en konstant
//	//dra liv kod
//	float LifeWidth = 0.5;
//	float LifeHeight = 0.125f;
//	std::array<int,4> LifeLayer = { 0,0,0,0 };
//	for (int i = 0; i < 4;i++)
//	{
//		LifeLayer[i] = GetComponent<SpriteRenderer>()->Layer[i];
//	}
//	LifeLayer[1] += 1;
//	float HealtbarPercentage = HP / MaxHp;
//	GetEngine().DrawTexture("Green.png", Vector2D(Position.x - (LifeWidth / 2 - (LifeWidth / 2) * HealtbarPercentage), Position.y + 0.5f), LifeWidth * HealtbarPercentage, LifeHeight, LifeLayer);
//	GetEngine().DrawTexture("RedSquare.png", Vector2D(Position.x + (LifeWidth / 2 - (LifeWidth / 2) * (1 - HealtbarPercentage)), Position.y + 0.5f), LifeWidth * (1 - HealtbarPercentage), LifeHeight, //LifeLayer);
//}
//void Enemy::DrawHealthbar(float Offset)
//{
//	//vi drar det med en konstant
//	//dra liv kod
//	float LifeWidth = 0.5;
//	float LifeHeight = 0.125f;
//	std::array<int,4> LifeLayer = { 0,0,0,0 };
//	for (int i = 0; i < 4;i++)
//	{
//		LifeLayer[i] = GetComponent<SpriteRenderer>()->Layer[i];
//	}
//	LifeLayer[1] += 1;
//
//	float HealtbarPercentage = HP / MaxHp;
//	GetEngine().DrawTexture("Green.png", Vector2D(Position.x - (LifeWidth / 2 - (LifeWidth / 2) * HealtbarPercentage), Position.y + 0.5f+Offset), LifeWidth * HealtbarPercentage, LifeHeight, //LifeLayer);
//	GetEngine().DrawTexture("RedSquare.png", Vector2D(Position.x + (LifeWidth / 2 - (LifeWidth / 2) * (1 - HealtbarPercentage)), Position.y + 0.5f+Offset), LifeWidth * (1 - HealtbarPercentage), //LifeHeight, LifeLayer);
//	//std::cout << MaxHp << std::endl;
//}