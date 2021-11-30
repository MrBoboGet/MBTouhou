#include <Johan.h>
#include <Engine\TextRendering.h>
#include <Enemy_Bullet_Template.h>
#include <SpelObjekt.h>
#include <Hitbox.h>
#include <random>
#include <iostream>
JohanAttack::JohanAttack()
{
	//
}
void JohanAttack::Update()
{
	//
}

JohanAttack::~JohanAttack()
{
	//
}
JohanCircelAroundAttack::JohanCircelAroundAttack(Johan* ObjectToAttachTo)
{
	Object = ObjectToAttachTo;
}
void JohanCircleAroundBulletUpdate(Enemy_Bullet_Template* Bullet)
{
	Bullet->Position = Bullet->Position + Vector2D(Bullet->Speed * std::cos(MBMath::DegreeToRadian(Bullet->Direction)), Bullet->Speed * std::sin(MBMath::DegreeToRadian(Bullet->Direction)));
	Player* Spelaren = static_cast<Player*>(TouhouEngine::FindObjectWithName("Spelaren"));
	if (Bullet == nullptr || Spelaren == nullptr)
	{
		return;
	}
	if (CollisionClass::Rectangle_Collision(Spelaren->Position,Spelaren->Hitbox,Spelaren->Rotation,Bullet->Position,Bullet->Hitbox,Bullet->Rotation))
	{
		Spelaren->GotHit = true;
		TouhouEngine::Destroy(Bullet);
	}
}
void JohanCircelAroundAttack::Update()
{
	if (!Finished)
	{
		if (!ShouldBeginToFinish)
		{
			//kod som gör att han börjar circla runt
			float DistanceTolerance = 0.01;
			if (BegunCircling == false)
			{
				Object->Position.x += 0.04;
				if (Object->Position.x >= CircleRadius)
				{
					Object->Position.x = CircleRadius;
					BegunCircling = true;
				}
			}
			else
			{
				CurrentAngle = fmod(CurrentAngle + CirclingAnglePerFrame, 360);
				Object->Position = CircleCenter + Vector2D(CircleRadius * std::cos(MBMath::DegreeToRadian(CurrentAngle)), CircleRadius * std::sin(MBMath::DegreeToRadian(CurrentAngle)));
			}
			Timer += 1;
			if (Timer % ShootAttackIntervall == 0)
			{
				//skut 8 skott åt alla håll
				int NumberOfShots = 16;
				for (size_t i = 0; i < NumberOfShots; i++)
				{
					Enemy_Bullet_Template* BulletToCreate = static_cast<Enemy_Bullet_Template*>(TouhouEngine::Create(new Enemy_Bullet_Template(Object->Position, "JohanCircleAroundBullet", "Enemy_Bullet", "fiendeattack1.png", 0.2, Vector2D(0.2, 0.2), JohanCircleAroundBulletUpdate)));
					BulletToCreate->Renderer.ColorKoef.G = 4;
					BulletToCreate->Speed = 0.07;
					BulletToCreate->Direction = (360 / (float)NumberOfShots) * i;
					BulletToCreate->Rotation = (360 / (float)NumberOfShots) * i - 90;
				}
			}
			//samtidigt skjuter vi saker från sidan med en satt vinkel är jämnt mellan rummt från topen till höger så att det blir lite mer spice
			if (Timer%ShootAttackIntervallSide == 0)
			{
				int NumberOfAttackes = 4;
				float DistanceBetweenAttacks = 2.7;
				float SpeedOfAttacks = 0.07;
				float Angle = 60;
				for (size_t i = 0; i < NumberOfAttackes; i++)
				{
					Enemy_Bullet_Template* BulletToCreate = static_cast<Enemy_Bullet_Template*>(TouhouEngine::Create(new Enemy_Bullet_Template(Object->Position, "JohanCircleAroundBullet", "Enemy_Bullet", "fiendeattack1.png", 0.2, Vector2D(0.2, 0.2), JohanCircleAroundBulletUpdate)));
					Enemy_Bullet_Template* BulletToCreate2 = static_cast<Enemy_Bullet_Template*>(TouhouEngine::Create(new Enemy_Bullet_Template(Object->Position, "JohanCircleAroundBullet", "Enemy_Bullet", "fiendeattack1.png", 0.2, Vector2D(0.2, 0.2), JohanCircleAroundBulletUpdate)));
					BulletToCreate->Position = Vector2D(4, 4.5 - i * DistanceBetweenAttacks);
					BulletToCreate2->Position = Vector2D(-4, 4.5 - i * DistanceBetweenAttacks);
					BulletToCreate->Speed = SpeedOfAttacks;
					BulletToCreate2->Speed = SpeedOfAttacks;
					BulletToCreate->Rotation = 180+Angle-90;
					BulletToCreate->Direction = 180+Angle;
					BulletToCreate2->Rotation = 360-Angle-90;
					BulletToCreate2->Direction = 360-Angle;
				}
			}
			if (Timer >= AttackLength)
			{
				ShouldBeginToFinish = true;
			}
		}
		else
		{
			//rör sig mot vår grund position
			if (Vector2D::DistanceToPoint(Object->Position,CircleCenter) > 0.04)
			{
				Object->Position = Object->Position + (CircleCenter - Object->Position) * 0.04;
			}
			else
			{
				Object->Position = CircleCenter;
				Finished = true;
				CurrentAngle = 0;
				ShouldBeginToFinish = false;
				BegunCircling = false;
				Timer = 0;
			}
		}
	}
}

JohanCircelAroundAttack::~JohanCircelAroundAttack()
{

}

JohanRightToLeftAttack::JohanRightToLeftAttack(Johan* ObjectToAttachTo)
{
	Object = ObjectToAttachTo;
}

void JohanRightToLeftAttack::Update()
{
	if (!Finished)
	{
		//kod som styr hur han åker
		Timer += 1;
		if (Timer < AttackLength)
		{
			if (std::abs(Object->Position.x + Direction * Speed) < JohanRange)
			{
				Object->Position.x = Object->Position.x + Direction * Speed;
			}
			else
			{
				Object->Position.x = JohanRange * MBMath::sign(Direction);
				Direction *= -1;
			}
			//kod som styr skapandet av projektiler
			if (Timer % BulletSpawnIntevall == 0)
			{
				std::uniform_real_distribution<float> PositionDistribution(BulletSpawnRange.x, BulletSpawnRange.y);
				std::uniform_real_distribution<float> AngleDistribution(270 - MaxAngelDeviance, 270 + MaxAngelDeviance);
				std::uniform_real_distribution<float> SpeedDistrubution(0, MaxSpeedDifferance);
				std::uniform_real_distribution<float> YDistribution(0, MaxYDifference);
				for (size_t i = 0; i < NumberOfBulletsToSpawn; i++)
				{
					Enemy_Bullet_Template* BulletToCreate = static_cast<Enemy_Bullet_Template*>(TouhouEngine::Create(new Enemy_Bullet_Template(Object->Position, "JohanCircleAroundBullet", "Enemy_Bullet", "fiendeattack1.png", 0.2, Vector2D(0.2, 0.2), JohanCircleAroundBulletUpdate)));
					BulletToCreate->Position = Vector2D(PositionDistribution(Generator), 4.5 - YDistribution(Generator));
					BulletToCreate->Direction = AngleDistribution(Generator);
					BulletToCreate->Rotation = BulletToCreate->Direction - 90;
					BulletToCreate->Speed = 0.07 + SpeedDistrubution(Generator);
				}
			}
		}
		else
		{
			if (std::abs(Object->Position.x + Direction * Speed) < 0)
			{
				Object->Position.x = Object->Position.x + Direction * Speed;
			}
			else
			{
				Object->Position.x = 0;
				Timer = 0;
				Finished = true;
			}
		}
	}
}

JohanRightToLeftAttack::~JohanRightToLeftAttack()
{
}
void MovingCircleBullet(Enemy_Bullet_Template* Bullet)
{
	Player* Spelaren = static_cast<Player*>(TouhouEngine::FindObjectWithName("Spelaren"));
	if (Bullet == nullptr || Spelaren == nullptr)
	{
		return;
	}
	//nu faktiskt va som ska hända
	//Vi tänker oss en cirkel som har en riktning, som vi sedan roterar runt
	//vi kan på rotationen av får projectil avgöra var någonstans på cirkeln den är
	//vi låter direction avgöra var någonstans cirkeln åker
	float CirleRadius = 2;
	float RotationSpeed = -2.3;
	float CircleSpeed = 0.025;
	float RotationDirection = -1;
	//först ändrar vi position för själva cirkel rörelsen
	Bullet->Position = Bullet->Position + Vector2D(std::cos(MBMath::DegreeToRadian(Bullet->Direction)) * CircleSpeed, std::sin(MBMath::DegreeToRadian(Bullet->Direction) * CircleSpeed));
	//vi kan med nuvarande vinkel räkna ut cirkelcentrum
	Vector2D DirectionToCenter = Vector2D(std::cos(MBMath::DegreeToRadian(Bullet->Rotation+90 + 90*RotationDirection)), std::sin(MBMath::DegreeToRadian(Bullet->Rotation+90 + 90*RotationDirection)));
	Vector2D CircleCenter = Bullet->Position + DirectionToCenter * CirleRadius;
	Bullet->Rotation = Bullet->Rotation + RotationSpeed;
	Bullet->Position = CircleCenter + Vector2D(std::cos(MBMath::DegreeToRadian(Bullet->Rotation + 90 - 90 * RotationDirection)), std::sin(MBMath::DegreeToRadian(Bullet->Rotation + 90 - 90 * RotationDirection)))*CirleRadius;
	if (CollisionClass::Rectangle_Collision(Spelaren->Position, Spelaren->Hitbox, Spelaren->Rotation, Bullet->Position, Bullet->Hitbox, Bullet->Rotation))
	{
		Spelaren->GotHit = true;
		TouhouEngine::Destroy(Bullet);
	}
}
JohanCirclingBulletAttack::JohanCirclingBulletAttack(Johan* ObjectToAttachTo)
{
	Object = ObjectToAttachTo;
}
void JohanCirclingBulletAttack::Update()
{
	if (!Finished)
	{
		//skapar en kula som flyger i cirkel grejen
		if (Timer%AttackDelay == 0)
		{
			//skapa i en cirkel
			int NumberOfBullets = 6;
			float CircleRadius = 2;
			int RotationDirection = -1;
			for (int j = 0; j < 2; j++)
			{
				Vector2D VectorAttAddera = Vector2D((4 * j)-2, 0);
				Vector2D CircleCenter = Object->Position + VectorAttAddera;
				for (int i = 0; i < NumberOfBullets; i++)
				{
					Enemy_Bullet_Template* BulletToCreate = static_cast<Enemy_Bullet_Template*>(TouhouEngine::Create(new Enemy_Bullet_Template(Object->Position, "JohanCircleAroundBullet", "Enemy_Bullet", "fiendeattack1.png", 0.2, Vector2D(0.2, 0.2), MovingCircleBullet)));
					BulletToCreate->Rotation = (360 / NumberOfBullets) * i;
					BulletToCreate->Direction = 270;
					BulletToCreate->Position = CircleCenter + Vector2D(std::cos(MBMath::DegreeToRadian(BulletToCreate->Rotation + 90 - 90 * RotationDirection)), std::sin(MBMath::DegreeToRadian(BulletToCreate->Rotation + 90 - 90 * RotationDirection))) * CircleRadius;
				}
			}
		}
		if (Timer%AttackDelay2 == 0)
		{
			for (int i = 0; i < 2; i++)
			{
				Vector2D Position = Vector2D(-2 + (4 * i), 2.7);
				for (int j = 0; j < 3; j++)
				{
					Enemy_Bullet_Template* BulletToCreate = static_cast<Enemy_Bullet_Template*>(TouhouEngine::Create(new Enemy_Bullet_Template(Object->Position, "JohanCircleAroundBullet", "Enemy_Bullet", "fiendeattack1.png", 0.2, Vector2D(0.2, 0.2), JohanCircleAroundBulletUpdate)));
					BulletToCreate->Renderer.ColorKoef.B = 4;
					BulletToCreate->Position = Position;
					BulletToCreate->Direction = 250 + 20 * j;
					BulletToCreate->Rotation = BulletToCreate->Direction - 90;
					BulletToCreate->Speed = 0.07;
				}
			}
		}
		Timer += 1;
		if (Timer >= AttackDuration)
		{
			Finished = true;
			Timer = 0;
		}
	}
}

JohanCirclingBulletAttack::~JohanCirclingBulletAttack()
{

}

Johan::Johan(Vector2D Position) : GameObject("Johan.png", 1.5)
{
	Tag = "Enemy";
	Name = "Johan";
	HP = MaxHp;
	this->Position = Position;
	Hitbox = Vector2D(1.5, 1.5);
	Attacker.push_back(new JohanCirclingBulletAttack(this));
	Attacker.push_back(new JohanCircelAroundAttack(this));
	Attacker.push_back(new JohanRightToLeftAttack(this));
}
void Johan::Update()
{
	if (StateStart)
	{
		//gör så att han åker ner ominously
		float PositionToStop = 2.7;
		if (Position.y != PositionToStop)
		{
			Position.y -= SpeedBeginning;
		}
		if (Position.y <= PositionToStop)
		{
			Position.y = PositionToStop;
			//nu spawnar healthbaren
			StateStart = false;
		}
		if (MaxHp > HP)
		{
			HP = MaxHp;
		}
	}
	else
	{
		//den generella loopen för hur han kommer göra sina attacker
		//här har vi också lite grejer som checka liv osv
		//rita Johans liv
		if (HP > 0)
		{
			std::array<int,4> LivLayern = { 100,0,0,0 };
			float HealthBarWidth = 6;
			float HealthBarHeight = 1;
			std::array<int,4> Layer2 = { 1001,0,0,0 };
			DrawTextRectangle("Johan", Vector2D(0, 4.2), Layer2, 0.4);
			TouhouEngine::DrawTexture("Green.png", Vector2D((1 - HP / (float)MaxHp) * HealthBarWidth * -0.5f, 3.8), 6 * (HP / (float)MaxHp), 0.1, LivLayern);
			TouhouEngine::DrawTexture("RedSquare.png", Vector2D((HealthBarWidth / 2) - (1 - HP / (float)MaxHp) * 0.5f * HealthBarWidth, 3.8), 6 * (1 - HP / (float)MaxHp), 0.1, LivLayern);
			JohanStartDelay -= 1;
			if (JohanStartDelay < 0)
			{
				//Vi likt förra spelet gör så att varje attack är objekt som vi kan sedan ha i en lista
				if (Attacker[CurrentAttackIndex]->Finished == false)
				{
					if (AttackDelayTimer < 0)
					{
						Attacker[CurrentAttackIndex]->Update();
					}
					AttackDelayTimer -= 1;
				}
				else
				{
					Attacker[CurrentAttackIndex]->Finished = false;
					CurrentAttackIndex = (CurrentAttackIndex + 1) % Attacker.size();
					//vad händer när´ett spellcard är finished
					std::vector<GameObject*> BulletsToDestroy = {};
					ActiveObjectIterator ObjectIterator = TouhouEngine::GetActiveObjectsIterator();
					while (ObjectIterator.HasEnded() == false)
					{
						if (ObjectIterator->GetTag() == "Enemy_Bullet")
						{
							BulletsToDestroy.push_back(*ObjectIterator);
						}
						ObjectIterator++;
					}
					
					//for (size_t i = 0; i < TouhouEngine::ActiveGameObjects.size(); i++)
					//{
					//	if (TouhouEngine::ActiveGameObjects[i]->GetTag() == "Enemy_Bullet")
					//	{
					//		BulletsToDestroy.push_back(TouhouEngine::ActiveGameObjects[i]);
					//	}
					//}
					for (size_t i = 0; i < BulletsToDestroy.size(); i++)
					{
						TouhouEngine::Destroy(BulletsToDestroy[i]);
					}
					AttackDelayTimer = AttackDelay;
					//play sound av att grejer förstörs
					//play ljud
				}
			}
		}
		else
		{
			//försör alla kulor
			std::vector<GameObject*> BulletsToDestroy = {};
			ActiveObjectIterator ObjectIterator = TouhouEngine::GetActiveObjectsIterator();
			while (ObjectIterator.HasEnded() == false)
			{
				if (ObjectIterator->GetTag() == "Enemy_Bullet")
				{
					BulletsToDestroy.push_back(*ObjectIterator);
				}
				ObjectIterator++;
			}
			//for (size_t i = 0; i < TouhouEngine::ActiveGameObjects.size(); i++)
			//{
			//	if (TouhouEngine::ActiveGameObjects[i]->GetTag() == "Enemy_Bullet")
			//	{
			//		BulletsToDestroy.push_back(TouhouEngine::ActiveGameObjects[i]);
			//	}
			//}
			for (size_t i = 0; i < BulletsToDestroy.size(); i++)
			{
				TouhouEngine::Destroy(BulletsToDestroy[i]);
			}
			DestroyTimer -= 1;
			if (DestroyTimer <= 0)
			{
				TouhouEngine::Destroy(this);
				TouhouEngine::PlaySound("Resources/Sounds/PlayerBomb.wav", 0.1);
			}
		}
	}
}
Johan::~Johan()
{

}