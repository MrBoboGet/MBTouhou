#include <SpelObjekt.h>
#include <Engine.h>
#include <iostream>
void Player::Player_Teleport()
{
	if (Key_Space)
	{
		if(KlickadeSpaceInnan == false)
		{
			if (CurrentEnergy >= TeleportEnergyConsumption)
			{
				CurrentEnergy -= TeleportEnergyConsumption;
				GetEngine().PlaySound("Resources/Sounds/AnimeTeleport.wav", 0.2, "SoundEffect");
				//resten av teleport koden

				//vi tpar oss beroende på vilka knappar vi håller in
				Vector2D Direction = Vector2D(0, 1);
				if (Key_Right)
				{
					Direction.x = 1;
					Direction.y = 0;
				}
				if (Key_Left)
				{
					Direction.x = -1;
					Direction.y = 0;
				}
				if (Key_Up)
				{
					Direction.y = 1;
				}
				if (Key_Down)
				{
					Direction.y = -1;
				}
				Direction = Direction.Normalized();
				//vi teleporterar oss sträckan
				Vector2D Position = Transform.GetPosition();
				Position = Vector2D(Position.x + Direction.x * TeleportDistance, Position.y + Direction.y * TeleportDistance);
				//nu förstör vi kulor i en viss radie
				//detta är verkligen inte självklart, men det vi gör nu är att vi loopar igenom alla objekt, kollar om dem är av en enemy kula, och är den nära nog förstör vi den
				std::vector<MBGameEngine::GameObjectReference> ObjectToDestroy = {};
				ActiveObjectIterator ObjectIterator = GetEngine().GetActiveObjectsIterator();
				while (ObjectIterator.HasEnded() == false)
				{
					if (ObjectIterator->GetTag() == "Enemy_Bullet")
					{
						if (Position.DistanceToPoint(ObjectIterator->Transform.GetPosition()) <= TeleportBulletDestructionRadius)
						{
							//nu ska vi förstöra föremålet
							//GetEngine().Destroy(*ObjectIterator);
							ObjectToDestroy.push_back(*ObjectIterator);
						}
					}
					ObjectIterator++;
				}
				for (size_t i = 0; i < ObjectToDestroy.size(); i++)
				{
					GetEngine().Destroy(ObjectToDestroy[i]);
				}
				//for (int i = 0; i < GetEngine().ActiveGameObjects.size(); i++)
				//{
				//	if (GetEngine().ActiveGameObjects[i]->GetTag() == "Enemy_Bullet")
				//	{
				//		if (Position.DistanceToPoint(GetEngine().ActiveGameObjects[i]->Position) <= TeleportBulletDestructionRadius)
				//		{
				//			//nu ska vi förstöra föremålet
				//			GetEngine().Destroy(GetEngine().ActiveGameObjects[i]);
				//		}
				//	}
				//}
				Transform.SetPosition(Position);
			}
		}
		KlickadeSpaceInnan = true;
	}
	else
	{
		KlickadeSpaceInnan = false;
	}
}