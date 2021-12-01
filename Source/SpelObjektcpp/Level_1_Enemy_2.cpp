#include<Level_1_Enemy_2.h>
#include<Enemy_Bullet_Template.h>
#include<iostream>
#include<SpelObjekt.h>
#include<math.h>
#include <Hitbox.h>
#include <Engine.h>
#include <MBUtility/MBMath.h>
//extern std::vector<GameObject*> ActiveGameObjects;
//extern std::vector<GameObject*> DeletedGameObjects;
Level_1_Enemy_2::Level_1_Enemy_2(Vector2D Plats, std::string Namn, std::string Tagg) : Enemy("Fiende2.png", 1.6)
{
	//AddComponent(new Rectangle_Hitbox());
	Position = Plats;
	SetName(Namn);
	SetTag(Tagg);
	//Hitbox = Vector2D(1.6, 1.6);
	speed = -0.005;
	HP = 10;
	MaxHp = HP;
	//std::cout << MaxHp << std::endl;
}
Level_1_Enemy_2::~Level_1_Enemy_2()
{
	
}
void Level_1_Enemy_2::OnCreate()
{
	GetComponent<Rectangle_Hitbox>()->Height = 1.6;
	GetComponent<Rectangle_Hitbox>()->Width = 1.6;
}
void Level_1_Enemy_2::Update()
{
	Position.y += speed;
	Level_1_Enemy_2_Timer += 1;
	if (Level_1_Enemy_2_Timer == 60)
	{
		for (int i = 0; i < 6; i++)
		{
			GameObject* kula = new Enemy_Bullet_Template(Position, "Enemy_Bullet", "Enemy_Bullet", "fiendeattack1.png", 0.16, Vector2D(0.16, 0.16),*Enemy_2_Kul_Logik);
			kula->Renderer.ColorKoef.G = 4;
			Enemy_Bullet_Template* KulanRiktiga = (static_cast<Enemy_Bullet_Template*>(kula));
			KulanRiktiga->Direction = 0 + 60 * i;
			KulanRiktiga->Speed = 0.02;
			KulanRiktiga->Rotation = KulanRiktiga->Direction - 90;
			TouhouEngine::Create(kula);
			//TouhouEngine::ActiveGameObjects.push_back(kula);
		}

		Level_1_Enemy_2_Timer = 0;
	}
	if (HP <= 0)
	{
		TouhouEngine::Destroy(this);
	}
	//std::cout << MaxHp << std::endl;
	DrawHealthbar(1.0f);
}
void Enemy_2_Kul_Logik(Enemy_Bullet_Template* Pointern)
{
	//av någon anledning fick vi fel när vi ändrade på det här sättet, får kolla upp mer noggrant, kan vara så att vi bara får värdet när vi gör så här
	//Enemy_Bullet_Template Kula = *Pointern;
	//	std::cout << Pointern->Position.x << " " << Kula.Position.x << std::endl;
	//här rör den sig i längst x axeln så att säga, i förhållande till vinkeln som blir den vi nollställer på så att säga
	Pointern->Position.x += Pointern->Speed * std::cos(MBMath::DegreeToRadian(Pointern->Direction));
	Pointern->Position.y += Pointern->Speed * std::sin(MBMath::DegreeToRadian(Pointern->Direction));
	//Nu tänker vi oss att efter ett steg i x axeln, så ändrar vi y pga derivatan i den nuvarande punkten, som vi får utav timern på fienden
	//den sinus funktion vi sätter in avgör därmed hur den ska röra sig
	//funktionen vi tar nu är bara sinus
	float Deriv = (4*std::sin(MBMath::DegreeToRadian(Pointern->Timer*6 + 0.01)) - 4*std::sin(MBMath::DegreeToRadian(Pointern->Timer*6))) / 0.01;
	//std::cout << Deriv << " "<<Pointern->Timer<<  std::endl;

	//blev lite lat och bara använde ett library, får kolla upp hur det faktiskt funkar sen
	Pointern->Rotation = (Pointern->Direction + -1*(180.0f / MBMath::Pi) * std::asin(Deriv / (std::sqrt((Pointern->Speed)*(Pointern->Speed) + Deriv * Deriv)))-90);
	//std::cout << Pointern->Direction<<" " << Pointern->Rotation << " " << (180.0f / Math::Pi) * asin(Deriv / (Math::sqrt((Pointern->Speed)*Pointern->Speed + Deriv * Deriv)))<<" "<<Deriv << std::endl;
	//nu ska vi lägga till så det blir rätt ortogonalt
	Pointern->Position.x += std::cos(MBMath::DegreeToRadian(Pointern->Direction - 90)) * Deriv;
	Pointern->Position.y += std::sin(MBMath::DegreeToRadian(Pointern->Direction - 90)) * Deriv;
	Pointern->Timer += 1;


	//kollision kod
	//int PlayerObjectPosition = 0;
	//for (int i = 0; i < TouhouEngine::ActiveGameObjects.size(); i++)
	//{
	//	if (TouhouEngine::ActiveGameObjects[i]->GetTag() == "Player")
	//	{
	//		PlayerObjectPosition = i;
	//		break;
	//	}
	//}
	GameObject* PlayerObject = TouhouEngine::FindObjectWithTag("Player");
	if (PlayerObject == nullptr)
	{
		return;
	}
	//uppdaterar med bra kolliksion kod
	//if (CollisionClass::Rectangle_Collision(PlayerObject->Position,PlayerObject->Hitbox,PlayerObject->Rotation,Pointern->Position,Pointern->Hitbox,Pointern->Rotation))
	if (Rectangle_Hitbox::Collides(PlayerObject->GetComponent<Rectangle_Hitbox>(),Pointern->GetComponent<Rectangle_Hitbox>()))
	{
		//void* Player_Pointer_Void = PlayerObject;
		Player* Player_Pointer = dynamic_cast<Player*>(PlayerObject); //fixa det här, väldigt jank
		Player_Pointer->GotHit = 1;
		
		TouhouEngine::Destroy(Pointern);
	}
	/*
	if (PlayerObject->Position.x - PlayerObject->Hitbox.x / 2 <= Pointern->Position.x + Pointern->Hitbox.x / 2 && PlayerObject->Position.x + PlayerObject->Hitbox.x / 2 >= Pointern->Position.x - Pointern->Hitbox.x / 2)
	{
		if (PlayerObject->Position.y - PlayerObject->Hitbox.y / 2 <= Pointern->Position.y + Pointern->Hitbox.y / 2 && PlayerObject->Position.y + PlayerObject->Hitbox.y / 2 >= Pointern->Position.y - Pointern->Hitbox.y / 2)
		{
			void* Player_Pointer_Void = ActiveGameObjects[PlayerObjectPosition];
			Player* Player_Pointer{ static_cast<Player*>(Player_Pointer_Void) };
			Player_Pointer->GotHit = 1;

			int PositionIListan = 0;
			for (int i = 0;i < ActiveGameObjects.size(); i++)
			{
				if (ActiveGameObjects[i] == Pointern)
				{
					PositionIListan = i;
					break;
				}
			}
			ActiveGameObjects.erase(ActiveGameObjects.begin() + PositionIListan);
			DeletedGameObjects.push_back(Pointern);
		}
	}
	*/
}