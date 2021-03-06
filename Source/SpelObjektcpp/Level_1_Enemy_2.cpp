#include<Level_1_Enemy_2.h>
#include<Enemy_Bullet_Template.h>
#include<iostream>
#include<SpelObjekt.h>
#include<math.h>
#include <Hitbox.h>
#include <Engine.h>
#include <MBUtility/MBMath.h>
#include <MBTouhouTypes.h>
//extern std::vector<GameObject*> ActiveGameObjects;
//extern std::vector<GameObject*> DeletedGameObjects;
Level_1_Enemy_2::Level_1_Enemy_2(Vector2D Plats, std::string Namn, std::string Tagg)// : Enemy("Fiende2.png", 1.6)
{
	//AddComponent(new Rectangle_Hitbox());
	Transform.SetPosition(Plats);
	SetName(Namn);
	SetTag(Tagg);
	//Hitbox = Vector2D(1.6, 1.6);
	speed = -0.005;
	GetComponent<MBTouhouEnemy_HP>()->HP = 10;
	GetComponent<MBTouhouEnemy_HP>()->MaxHP = 10;
	m_TextureName = "Fiende2.png";
	GetComponent<SpriteRenderer>()->Width = 1.6;
	//std::cout << MaxHp << std::endl;
}
Level_1_Enemy_2::~Level_1_Enemy_2()
{
	
}
void Level_1_Enemy_2::OnCreate()
{
	GetComponent<Rectangle_Hitbox>()->Height = 1.6;
	GetComponent<Rectangle_Hitbox>()->Width = 1.6;
	GetComponent<SpriteRenderer>()->SpriteTexture = GetEngine().LoadNamedTexture(m_TextureName, "Resources/SpelResurser/Sprites/"+m_TextureName);
	GetComponent<MBTouhouEnemy_HP>()->HealthBarOffset = 1;
}
void Level_1_Enemy_2::Update()
{
	Transform.SetPosition(Transform.GetPosition() + MBMath::MBVector3<float>(0,speed,0));
	Level_1_Enemy_2_Timer += 1;
	if (Level_1_Enemy_2_Timer == 60)
	{
		for (int i = 0; i < 6; i++)
		{
			GameObject* kula = new Enemy_Bullet_Template(Transform.GetPosition(), "Enemy_Bullet", "Enemy_Bullet", "fiendeattack1.png", 0.16, Vector2D(0.16, 0.16),*Enemy_2_Kul_Logik);
			kula->GetComponent<SpriteRenderer>()->ColorKoef.G = 4;
			Enemy_Bullet_Template* KulanRiktiga = (static_cast<Enemy_Bullet_Template*>(kula));
			KulanRiktiga->Direction = 0 + 60 * i;
			KulanRiktiga->Speed = 0.02;
			KulanRiktiga->Transform.SetRotation(MBMath::MBVector3<float>(KulanRiktiga->Direction - 90,0,0));
			GetEngine().Create(kula);
			//GetEngine().ActiveGameObjects.push_back(kula);
		}

		Level_1_Enemy_2_Timer = 0;
	}
	//if (HP <= 0)
	//{
	//	GetEngine().Destroy(this);
	//}
	//std::cout << MaxHp << std::endl;
	//DrawHealthbar(1.0f);
}
void Enemy_2_Kul_Logik(MBGameEngine::MBGameEngine& Engine,Enemy_Bullet_Template* Pointern)
{
	//av n?gon anledning fick vi fel n?r vi ?ndrade p? det h?r s?ttet, f?r kolla upp mer noggrant, kan vara s? att vi bara f?r v?rdet n?r vi g?r s? h?r
	//Enemy_Bullet_Template Kula = *Pointern;
	//	std::cout << Pointern->Position.x << " " << Kula.Position.x << std::endl;
	//h?r r?r den sig i l?ngst x axeln s? att s?ga, i f?rh?llande till vinkeln som blir den vi nollst?ller p? s? att s?ga
	Vector2D NewPosition = Pointern->Transform.GetPosition();
	NewPosition.x += Pointern->Speed * std::cos(MBMath::DegreeToRadian(Pointern->Direction));
	NewPosition.y += Pointern->Speed * std::sin(MBMath::DegreeToRadian(Pointern->Direction));
	//Nu t?nker vi oss att efter ett steg i x axeln, s? ?ndrar vi y pga derivatan i den nuvarande punkten, som vi f?r utav timern p? fienden
	//den sinus funktion vi s?tter in avg?r d?rmed hur den ska r?ra sig
	//funktionen vi tar nu ?r bara sinus
	float Deriv = (4*std::sin(MBMath::DegreeToRadian(Pointern->Timer*6 + 0.01)) - 4*std::sin(MBMath::DegreeToRadian(Pointern->Timer*6))) / 0.01;
	//std::cout << Deriv << " "<<Pointern->Timer<<  std::endl;

	//blev lite lat och bara anv?nde ett library, f?r kolla upp hur det faktiskt funkar sen
	Pointern->Transform.SetRotation(MBMath::MBVector3<float>(Pointern->Direction + -1*(180.0f / MBMath::Pi) * std::asin(Deriv / (std::sqrt((Pointern->Speed)*(Pointern->Speed) + Deriv * Deriv)))-90,0,0));
	//std::cout << Pointern->Direction<<" " << Pointern->Rotation << " " << (180.0f / Math::Pi) * asin(Deriv / (Math::sqrt((Pointern->Speed)*Pointern->Speed + Deriv * Deriv)))<<" "<<Deriv << std::endl;
	//nu ska vi l?gga till s? det blir r?tt ortogonalt
	NewPosition.x += std::cos(MBMath::DegreeToRadian(Pointern->Direction - 90)) * Deriv;
	NewPosition.y += std::sin(MBMath::DegreeToRadian(Pointern->Direction - 90)) * Deriv;
	Pointern->Timer += 1;

	Pointern->Transform.SetPosition(NewPosition);

	//kollision kod
	//int PlayerObjectPosition = 0;
	//for (int i = 0; i < GetEngine().ActiveGameObjects.size(); i++)
	//{
	//	if (GetEngine().ActiveGameObjects[i]->GetTag() == "Player")
	//	{
	//		PlayerObjectPosition = i;
	//		break;
	//	}
	//}
	MBGameEngine::GameObjectReference PlayerObject = Engine.FindObjectWithTag("Player");
	if (PlayerObject == nullptr)
	{
		return;
	}
	//uppdaterar med bra kolliksion kod
	//if (CollisionClass::Rectangle_Collision(PlayerObject->Position,PlayerObject->Hitbox,PlayerObject->Rotation,Pointern->Position,Pointern->Hitbox,Pointern->Rotation))
	if (Rectangle_Hitbox::Collides(PlayerObject->GetComponent<Rectangle_Hitbox>(),Pointern->GetComponent<Rectangle_Hitbox>()))
	{
		//void* Player_Pointer_Void = PlayerObject;
		//Player* Player_Pointer = dynamic_cast<Player*>(PlayerObject); //fixa det h?r, v?ldigt jank
		PlayerObject->GetComponent<TouhouPlayer_HP>()->RegisterCollision();
		
		Engine.Destroy(Pointern);
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