#include <Level_1_Enemy_3.h>
#include <Enemy_Bullet_Template.h>
#include <Engine.h>
#include <Hitbox.h>
#include <SpelObjekt.h>
#include <iostream>
Level_1_Enemy_3::Level_1_Enemy_3(Vector2D Position) : Enemy("Fiende3.png", 0.8)
{
	//inget behöver hända, vi gör alla grejer i själva class beskrivningen
	//fäörutom psoitioon doe
	this->Position = Position;
	//Hitbox = Vector2D(0.8, 0.8);
	SetName("Level_1_Enemy_3");
	SetTag("Enemy");
	HP = 10;
	MaxHp = HP;
}
void Level_1_Enemy_3::OnCreate()
{
	GetComponent<Rectangle_Hitbox>()->Height = 0.8;
	GetComponent<Rectangle_Hitbox>()->Width = 0.8;
}
Level_1_Enemy_3::~Level_1_Enemy_3()
{
}
void Enemy_3_KulLogik(Enemy_Bullet_Template* Kula);
//	Enemy_Bullet_Template(Vector2D Plats, std::string Namn, std::string Tagg,std::string Bild, float Storlek, Vector2D hitplox,void(*Update)(Enemy_Bullet_Template*));
void Level_1_Enemy_3::Update()
{
	//vi rör oss neråt tills vi kommer till en del av skärmen, där vi stannar och börjar sikta och charga mot spelaren
	if (Position.y - Speed > /*Detta är toppen av skrämen i koordianter*/5 -DistanceFromTopToStop)
	{
		Position.y -= Speed;
	}
	else
	{
		Position.y = 5 - DistanceFromTopToStop;
	}
	//om vi nu är vid det rätta stäkllet börjar vi ladda
	if (Position.y == 5-DistanceFromTopToStop)
	{
		//börja ladda skottet
		Timer += 1;
		if (Timer == ChargeTimeInFrames)
		{
			//vi har laddat klart, nu ska vi skjuta skottet
			//vi skapar alltså den kula som har egenskapen vi vill ha
			Enemy_Bullet_Template* Kula =static_cast<Enemy_Bullet_Template*>(TouhouEngine::Create(new Enemy_Bullet_Template(Position, "Enemy_Bullet", "Enemy_Bullet", "fiendeattack1.png", 0.2f, Vector2D(0.2, 0.2), *Enemy_3_KulLogik)));
			Kula->Renderer.ColorKoef.B = 3;
			Kula->Renderer.ColorKoef.R = 3;
			Kula->Renderer.ColorKoef.G = 3;
			Kula->Speed = BulletSpeed;
			Timer = 0;
		}
	}
	if (HP <= 0)
	{
		TouhouEngine::Destroy(static_cast<GameObject*>(this));
	}
	DrawHealthbar();
}
void Enemy_3_KulLogik(Enemy_Bullet_Template* Kula)
{
	//den rör sig mot spelaren
	GameObject* Spelaren =  TouhouEngine::FindObjectWithName("Spelaren");
	//använder timern som en "har den satts igång funktion
	if (Spelaren == nullptr)
	{
		return;
	}
	if (Kula->Timer == 0)
	{
		//sätter dess riktning nu
		Kula->Direction = Vector2D::AngleFromXAxis(Spelaren->Position- Kula->Position);
		Kula->Rotation = Kula->Direction-90;
		Kula->Timer += 1;
	}
	Kula->Position.x += std::cos(MBMath::DegreeToRadian(Kula->Direction)) * Kula->Speed;
	Kula->Position.y += std::sin(MBMath::DegreeToRadian(Kula->Direction)) * Kula->Speed;

	//uppdaterar med bra kolliksion kod
	if (Rectangle_Hitbox::Collides(Spelaren->GetComponent<Rectangle_Hitbox>(), Kula->GetComponent<Rectangle_Hitbox>()))
	{
		void* Player_Pointer_Void = Spelaren;
		Player* Player_Pointer = { static_cast<Player*>(Player_Pointer_Void) };
		Player_Pointer->GotHit = 1;
		std::cout << Kula->Position.x << " " << Kula->Position.y << " " << Spelaren->Position.x << " " << Spelaren->Position.y << std::endl;
		TouhouEngine::Destroy(Kula);
	}
}