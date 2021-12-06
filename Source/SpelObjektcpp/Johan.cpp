#include <Johan.h>
#include <Engine\TextRendering.h>
#include <Enemy_Bullet_Template.h>
#include <SpelObjekt.h>
#include <Hitbox.h>
#include <random>
#include <iostream>


//BEGIN JohanHP
void JohanHP::Update()
{

}
//END JohanHP


MBGameEngine::MBGameEngine& JohanAttack::GetEngine()
{
	return(Object->GetEngine());
}
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
void JohanCircleAroundBulletUpdate(MBGameEngine::MBGameEngine& Engine,Enemy_Bullet_Template* Bullet)
{
	Bullet->Transform.SetPosition(Vector2D(Bullet->Transform.GetPosition())+ Vector2D(Bullet->Speed * std::cos(MBMath::DegreeToRadian(Bullet->Direction)), Bullet->Speed * std::sin(MBMath::DegreeToRadian(Bullet->Direction))));
	MBGameEngine::ObjectReference<Player> Spelaren = Engine.FindObjectWithName("Spelaren");
	if (Bullet == nullptr || Spelaren == nullptr)
	{
		return;
	}
	//if (Rectangle_Hitbox::Rectangle_Collision(Spelaren->Position,Spelaren->Hitbox,Spelaren->Rotation,Bullet->Position,Bullet->Hitbox,Bullet->Rotation))
	if (Rectangle_Hitbox::Collides(Spelaren->GetComponent<Rectangle_Hitbox>(),Bullet->GetComponent<Rectangle_Hitbox>()))
	{
		//Spelaren->GotHit = true;
		Spelaren->GetComponent<TouhouPlayer_HP>()->RegisterCollision();
		Engine.Destroy(Bullet);
	}
}
void JohanCircelAroundAttack::Update()
{
	Vector2D JohanPosition = Object->Transform.GetPosition();
	if (!Finished)
	{
		if (!ShouldBeginToFinish)
		{
			//kod som gör att han börjar circla runt
			float DistanceTolerance = 0.01;
			if (BegunCircling == false)
			{
				JohanPosition.x += 0.04;
				if (JohanPosition.x >= CircleRadius)
				{
					JohanPosition.x = CircleRadius;
					BegunCircling = true;
				}
			}
			else
			{
				CurrentAngle = fmod(CurrentAngle + CirclingAnglePerFrame, 360);
				JohanPosition = CircleCenter + Vector2D(CircleRadius * std::cos(MBMath::DegreeToRadian(CurrentAngle)), CircleRadius * std::sin(MBMath::DegreeToRadian(CurrentAngle)));
			}
			Timer += 1;
			if (Timer % ShootAttackIntervall == 0)
			{
				//skut 8 skott åt alla håll
				int NumberOfShots = 16;
				for (size_t i = 0; i < NumberOfShots; i++)
				{
					MBGameEngine::ObjectReference<Enemy_Bullet_Template> BulletToCreate = GetEngine().Create(new Enemy_Bullet_Template(JohanPosition, "JohanCircleAroundBullet", "Enemy_Bullet", "fiendeattack1.png", 0.2, Vector2D(0.2, 0.2), JohanCircleAroundBulletUpdate));
					BulletToCreate->GetComponent<SpriteRenderer>()->ColorKoef.G = 4;
					BulletToCreate->Speed = 0.07;
					BulletToCreate->Direction = (360 / (float)NumberOfShots) * i;
					BulletToCreate->Transform.SetRotation(MBMath::MBVector3<float>((360 / (float)NumberOfShots) * i - 90,0,0));
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
					MBGameEngine::ObjectReference<Enemy_Bullet_Template> BulletToCreate = GetEngine().Create(new Enemy_Bullet_Template(JohanPosition, "JohanCircleAroundBullet", "Enemy_Bullet", "fiendeattack1.png", 0.2, Vector2D(0.2, 0.2), JohanCircleAroundBulletUpdate));
					MBGameEngine::ObjectReference<Enemy_Bullet_Template> BulletToCreate2 = GetEngine().Create(new Enemy_Bullet_Template(JohanPosition, "JohanCircleAroundBullet", "Enemy_Bullet", "fiendeattack1.png", 0.2, Vector2D(0.2, 0.2), JohanCircleAroundBulletUpdate));
					BulletToCreate->Transform.SetPosition(Vector2D(4, 4.5 - i * DistanceBetweenAttacks));
					BulletToCreate2->Transform.SetPosition(Vector2D(-4, 4.5 - i * DistanceBetweenAttacks));
					BulletToCreate->Speed = SpeedOfAttacks;
					BulletToCreate2->Speed = SpeedOfAttacks;
					BulletToCreate->Transform.SetRotation(MBMath::MBVector3<float>(180 + Angle - 90,0,0));
					BulletToCreate->Direction = 180+Angle;
					BulletToCreate2->Transform.SetRotation(MBMath::MBVector3<float>(360-Angle-90,0,0));
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
			if (Vector2D::DistanceToPoint(JohanPosition,CircleCenter) > 0.04)
			{
				JohanPosition = JohanPosition + (CircleCenter - JohanPosition) * 0.04;
			}
			else
			{
				JohanPosition = CircleCenter;
				Finished = true;
				CurrentAngle = 0;
				ShouldBeginToFinish = false;
				BegunCircling = false;
				Timer = 0;
			}
		}
	}
	Object->Transform.SetPosition(JohanPosition);
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
		Vector2D JohanPosition = Object->Transform.GetPosition();
		Timer += 1;
		if (Timer < AttackLength)
		{
			if (std::abs(JohanPosition.x + Direction * Speed) < JohanRange)
			{
				JohanPosition.x = JohanPosition.x + Direction * Speed;
			}
			else
			{
				JohanPosition.x = JohanRange * MBMath::sign(Direction);
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
					MBGameEngine::ObjectReference<Enemy_Bullet_Template> BulletToCreate = GetEngine().Create(new Enemy_Bullet_Template(JohanPosition, "JohanCircleAroundBullet", "Enemy_Bullet", "fiendeattack1.png", 0.2, Vector2D(0.2, 0.2), JohanCircleAroundBulletUpdate));
					BulletToCreate->Transform.SetPosition(Vector2D(PositionDistribution(Generator), 4.5 - YDistribution(Generator)));
					BulletToCreate->Direction = AngleDistribution(Generator);
					BulletToCreate->Transform.SetRotation(MBMath::MBVector3<float>(BulletToCreate->Direction - 90,0,0));
					BulletToCreate->Speed = 0.07 + SpeedDistrubution(Generator);
				}
			}
		}
		else
		{
			if (std::abs(JohanPosition.x + Direction * Speed) < 0)
			{
				JohanPosition.x = JohanPosition.x + Direction * Speed;
			}
			else
			{
				JohanPosition.x = 0;
				Timer = 0;
				Finished = true;
			}
		}
		Object->Transform.SetPosition(JohanPosition);
	}
}

JohanRightToLeftAttack::~JohanRightToLeftAttack()
{
}
void MovingCircleBullet(MBGameEngine::MBGameEngine& Engine,Enemy_Bullet_Template* Bullet)
{
	MBGameEngine::ObjectReference<Player> Spelaren = Engine.FindObjectWithName("Spelaren");
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
	Vector2D BulletPosition = Bullet->Transform.GetPosition();
	float BulletRotation = Bullet->Transform.GetRotation()[0];
	//först ändrar vi position för själva cirkel rörelsen
	//float Debug = std::sin(MBMath::DegreeToRadian(Bullet->Direction) * CircleSpeed;
	BulletPosition = BulletPosition + Vector2D(std::cos(MBMath::DegreeToRadian(Bullet->Direction)) * CircleSpeed, std::sin(MBMath::DegreeToRadian(Bullet->Direction)) * CircleSpeed);
	//vi kan med nuvarande vinkel räkna ut cirkelcentrum
	Vector2D DirectionToCenter = Vector2D(std::cos(MBMath::DegreeToRadian(BulletRotation +90 + (90*RotationDirection))), std::sin(MBMath::DegreeToRadian(BulletRotation +90 + (90*RotationDirection))));
	Vector2D CircleCenter = BulletPosition + DirectionToCenter * CirleRadius;
	BulletRotation = BulletRotation + RotationSpeed;
	BulletPosition = CircleCenter + Vector2D(std::cos(MBMath::DegreeToRadian(BulletRotation + 90 - (90 * RotationDirection))), std::sin(MBMath::DegreeToRadian(BulletRotation + 90 - (90 * RotationDirection))))*CirleRadius;
	if (Rectangle_Hitbox::Collides(Spelaren->GetComponent<Rectangle_Hitbox>(), Bullet->GetComponent<Rectangle_Hitbox>()))
	{
		//Spelaren->GotHit = true;
		Spelaren->GetComponent<TouhouPlayer_HP>()->RegisterCollision();
		Engine.Destroy(Bullet);
	}
	Bullet->Transform.SetPosition(BulletPosition);
	Bullet->Transform.SetRotation(MBMath::MBVector3<float>(BulletRotation, 0, 0));
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
		Vector2D JohanPosition = Object->Transform.GetPosition();
		if (Timer%AttackDelay == 0)
		{
			//skapa i en cirkel
			int NumberOfBullets = 6;
			float CircleRadius = 2;
			int RotationDirection = -1;
			for (int j = 0; j < 2; j++)
			{
				Vector2D VectorAttAddera = Vector2D((4 * j)-2, 0);
				Vector2D CircleCenter = JohanPosition + VectorAttAddera;
				for (int i = 0; i < NumberOfBullets; i++)
				{
					MBGameEngine::ObjectReference<Enemy_Bullet_Template> BulletToCreate = GetEngine().Create(new Enemy_Bullet_Template(JohanPosition, "JohanCircleAroundBullet", "Enemy_Bullet", "fiendeattack1.png", 0.2, Vector2D(0.2, 0.2), MovingCircleBullet));
					BulletToCreate->Transform.SetRotation(MBMath::MBVector3<float>((360 / NumberOfBullets) * i,0,0));
					BulletToCreate->Direction = 270;
					BulletToCreate->Transform.SetPosition(CircleCenter + Vector2D(std::cos(MBMath::DegreeToRadian(BulletToCreate->Transform.GetRotation()[0] + 90 - (90 * RotationDirection))), std::sin(MBMath::DegreeToRadian(BulletToCreate->Transform.GetRotation()[0] + 90 - (90 * RotationDirection)))) * CircleRadius);
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
					MBGameEngine::ObjectReference<Enemy_Bullet_Template> BulletToCreate = GetEngine().Create(new Enemy_Bullet_Template(JohanPosition, "JohanCircleAroundBullet", "Enemy_Bullet", "fiendeattack1.png", 0.2, Vector2D(0.2, 0.2), JohanCircleAroundBulletUpdate));
					BulletToCreate->GetComponent<SpriteRenderer>()->ColorKoef.B = 4;
					BulletToCreate->Transform.SetPosition(Position);
					BulletToCreate->Direction = 250 + 20 * j;
					BulletToCreate->Transform.SetRotation(MBMath::MBVector3<float>(BulletToCreate->Direction - 90,0,0));
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

Johan::Johan(Vector2D Position)// : GameObject("Johan.png", 1.5)
{
	SetTag("Enemy");
	SetName("Johan");
	AddComponent(new MBTouhouEnemy_HP());
	Transform.SetPosition(Position);
	//Hitbox = Vector2D(1.5, 1.5);
	AddComponent(new Rectangle_Hitbox());
	AddComponent(new SpriteRenderer());
	//AddComponent();
	Attacker.push_back(new JohanCirclingBulletAttack(this));
	Attacker.push_back(new JohanCircelAroundAttack(this));
	Attacker.push_back(new JohanRightToLeftAttack(this));
}
void Johan::OnCreate() 
{
	GetComponent<Rectangle_Hitbox>()->Width = 1.5;
	GetComponent<Rectangle_Hitbox>()->Height = 1.5;
	MBGameEngine::ObjectReference<SpriteRenderer> Renderer = GetComponent<SpriteRenderer>();
	Renderer->Width = 1.5;
	Renderer->SpriteTexture = GetEngine().LoadNamedTexture("Johan.png", "Resources/SpelResurser/Sprites/Johan.png");
	m_HPComponent = GetComponent<MBTouhouEnemy_HP>();
	m_HPComponent->HP = MaxHp;
}
void Johan::Update()
{
	Vector2D Position = Transform.GetPosition();
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
		if (MaxHp > m_HPComponent->HP)
		{
			m_HPComponent->HP = MaxHp;
		}
	}
	else
	{
		//den generella loopen för hur han kommer göra sina attacker
		//här har vi också lite grejer som checka liv osv
		//rita Johans liv
		if (m_HPComponent->HP > 0)
		{
			std::array<int,4> LivLayern = { 100,0,0,0 };
			float HealthBarWidth = 6;
			float HealthBarHeight = 1;
			std::array<int,4> Layer2 = { 1001,0,0,0 };
			DrawTextRectangle(GetEngine(),"Johan", Vector2D(0, 4.2), Layer2, 0.4);
			GetEngine().DrawTexture("Green.png", Vector2D((1 - m_HPComponent->HP / (float)MaxHp) * HealthBarWidth * -0.5f, 3.8), 6 * (m_HPComponent->HP / (float)MaxHp), 0.1, LivLayern);
			GetEngine().DrawTexture("RedSquare.png", Vector2D((HealthBarWidth / 2) - (1 - m_HPComponent->HP / (float)MaxHp) * 0.5f * HealthBarWidth, 3.8), 6 * (1 - m_HPComponent->HP / (float)MaxHp), 0.1, LivLayern);
			JohanStartDelay -= 1;
			if (JohanStartDelay < 0)
			{
				//Vi likt förra spelet gör så att varje attack är objekt som vi kan sedan ha i en lista
				if (Attacker[CurrentAttackIndex]->Finished == false)
				{
					if (AttackDelayTimer < 0)
					{
						Attacker[CurrentAttackIndex]->Update();
						Position = Transform.GetPosition();
					}
					AttackDelayTimer -= 1;
				}
				else
				{
					Attacker[CurrentAttackIndex]->Finished = false;
					CurrentAttackIndex = (CurrentAttackIndex + 1) % Attacker.size();
					//vad händer när´ett spellcard är finished
					std::vector<MBGameEngine::GameObjectReference> BulletsToDestroy = {};
					ActiveObjectIterator ObjectIterator = GetEngine().GetActiveObjectsIterator();
					while (ObjectIterator.HasEnded() == false)
					{
						if (ObjectIterator->GetTag() == "Enemy_Bullet")
						{
							BulletsToDestroy.push_back(*ObjectIterator);
						}
						ObjectIterator++;
					}
					
					//for (size_t i = 0; i < GetEngine().ActiveGameObjects.size(); i++)
					//{
					//	if (GetEngine().ActiveGameObjects[i]->GetTag() == "Enemy_Bullet")
					//	{
					//		BulletsToDestroy.push_back(GetEngine().ActiveGameObjects[i]);
					//	}
					//}
					for (size_t i = 0; i < BulletsToDestroy.size(); i++)
					{
						GetEngine().Destroy(BulletsToDestroy[i]);
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
			std::vector<MBGameEngine::GameObjectReference> BulletsToDestroy = {};
			ActiveObjectIterator ObjectIterator = GetEngine().GetActiveObjectsIterator();
			while (ObjectIterator.HasEnded() == false)
			{
				if (ObjectIterator->GetTag() == "Enemy_Bullet")
				{
					BulletsToDestroy.push_back(*ObjectIterator);
				}
				ObjectIterator++;
			}
			//for (size_t i = 0; i < GetEngine().ActiveGameObjects.size(); i++)
			//{
			//	if (GetEngine().ActiveGameObjects[i]->GetTag() == "Enemy_Bullet")
			//	{
			//		BulletsToDestroy.push_back(GetEngine().ActiveGameObjects[i]);
			//	}
			//}
			for (size_t i = 0; i < BulletsToDestroy.size(); i++)
			{
				GetEngine().Destroy(BulletsToDestroy[i]);
			}
			DestroyTimer -= 1;
			if (DestroyTimer <= 0)
			{
				GetEngine().Destroy(this);
				GetEngine().PlaySound("Resources/Sounds/PlayerBomb.wav", 0.1);
			}
		}
	}
	Transform.SetPosition(Position);
}
Johan::~Johan()
{

}