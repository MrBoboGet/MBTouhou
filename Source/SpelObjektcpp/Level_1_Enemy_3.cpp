#include <Level_1_Enemy_3.h>
#include <Enemy_Bullet_Template.h>
#include <Engine.h>
#include <Hitbox.h>
#include <SpelObjekt.h>
#include <iostream>
#include <MBTouhouTypes.h>
Level_1_Enemy_3::Level_1_Enemy_3(Vector2D Position)// : Enemy("Fiende3.png", 0.8)
{
	//inget beh?ver h?nda, vi g?r alla grejer i sj?lva class beskrivningen
	//f??rutom psoitioon doe
	Transform.SetPosition(Position);
	//Hitbox = Vector2D(0.8, 0.8);
	SetName("Level_1_Enemy_3");
	SetTag("Enemy");
	GetComponent<SpriteRenderer>()->Width = 0.8;
	m_TextureName = "Fiende3.png";
	//AddComponent(new MBTouhouEnemy_HP());
	GetComponent<MBTouhouEnemy_HP>()->HP = 10;
	GetComponent<MBTouhouEnemy_HP>()->MaxHP = 10;
	//MaxHp = HP;
}
void Level_1_Enemy_3::OnCreate()
{
	GetComponent<Rectangle_Hitbox>()->Height = 0.8;
	GetComponent<Rectangle_Hitbox>()->Width = 0.8;
	GetComponent<SpriteRenderer>()->SpriteTexture = GetEngine().LoadNamedTexture(m_TextureName, "Resources/SpelResurser/Sprites/" + m_TextureName);
}
Level_1_Enemy_3::~Level_1_Enemy_3()
{
}
void Enemy_3_KulLogik(MBGameEngine::MBGameEngine&, Enemy_Bullet_Template* Kula);
//	Enemy_Bullet_Template(Vector2D Plats, std::string Namn, std::string Tagg,std::string Bild, float Storlek, Vector2D hitplox,void(*Update)(Enemy_Bullet_Template*));
void Level_1_Enemy_3::Update()
{
	Vector2D Position = Transform.GetPosition();
	//vi r?r oss ner?t tills vi kommer till en del av sk?rmen, d?r vi stannar och b?rjar sikta och charga mot spelaren
	if (Position.y - Speed > /*Detta ?r toppen av skr?men i koordianter*/5 -DistanceFromTopToStop)
	{
		Position.y -= Speed;
	}
	else
	{
		Position.y = 5 - DistanceFromTopToStop;
	}
	//om vi nu ?r vid det r?tta st?kllet b?rjar vi ladda
	if (Position.y == 5-DistanceFromTopToStop)
	{
		//b?rja ladda skottet
		Timer += 1;
		if (Timer == ChargeTimeInFrames)
		{
			//vi har laddat klart, nu ska vi skjuta skottet
			//vi skapar allts? den kula som har egenskapen vi vill ha
			MBGameEngine::ObjectReference<Enemy_Bullet_Template> Kula =GetEngine().Create(new Enemy_Bullet_Template(Position, "Enemy_Bullet", "Enemy_Bullet", "fiendeattack1.png", 0.2f, Vector2D(0.2, 0.2), *Enemy_3_KulLogik));
			Kula->GetComponent<SpriteRenderer>()->ColorKoef.B = 3;
			Kula->GetComponent<SpriteRenderer>()->ColorKoef.R = 3;
			Kula->GetComponent<SpriteRenderer>()->ColorKoef.G = 3;
			Kula->Speed = BulletSpeed;
			Timer = 0;
		}
	}
	Transform.SetPosition(Position);
	//if (HP <= 0)
	//{
	//	GetEngine().Destroy(static_cast<GameObject*>(this));
	//}
	//DrawHealthbar();
}
void Enemy_3_KulLogik(MBGameEngine::MBGameEngine& Engine, Enemy_Bullet_Template* Kula)
{
	//den r?r sig mot spelaren
	MBGameEngine::GameObjectReference Spelaren = Kula->GetEngine().FindObjectWithName("Spelaren");
	//anv?nder timern som en "har den satts ig?ng funktion
	if (Spelaren == nullptr)
	{
		return;
	}
	if (Kula->Timer == 0)
	{
		//s?tter dess riktning nu
		Kula->Direction = Vector2D::AngleFromXAxis(Spelaren->Transform.GetPosition()- Kula->Transform.GetPosition());
		Kula->Transform.SetRotation(MBMath::MBVector3<float>(Kula->Direction-90,0,0));
		Kula->Timer += 1;
	}
	Vector2D BulletPosition = Kula->Transform.GetPosition();
	BulletPosition.x += std::cos(MBMath::DegreeToRadian(Kula->Direction)) * Kula->Speed;
	BulletPosition.y += std::sin(MBMath::DegreeToRadian(Kula->Direction)) * Kula->Speed;

	//uppdaterar med bra kolliksion kod
	if (Rectangle_Hitbox::Collides(Spelaren->GetComponent<Rectangle_Hitbox>(), Kula->GetComponent<Rectangle_Hitbox>()))
	{
		//void* Player_Pointer_Void = Spelaren;
		//Player* Player_Pointer = { static_cast<Player*>(Player_Pointer_Void) };
		Spelaren->GetComponent<TouhouPlayer_HP>()->RegisterCollision();
		std::cout << BulletPosition.x << " " << BulletPosition.y << " " << Spelaren->Transform.GetPosition()[0] << " " << Spelaren->Transform.GetPosition()[1] << std::endl;
		Engine.Destroy(Kula);
	}
	Kula->Transform.SetPosition(BulletPosition);
}