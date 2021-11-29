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
				TouhouEngine::PlaySound("Resources/Sounds/AnimeTeleport.wav", 0.2, "SoundEffect");
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
				Position = Vector2D(Position.x + Direction.x * TeleportDistance, Position.y + Direction.y * TeleportDistance);
				//nu förstör vi kulor i en viss radie
				//detta är verkligen inte självklart, men det vi gör nu är att vi loopar igenom alla objekt, kollar om dem är av en enemy kula, och är den nära nog förstör vi den
				ActiveObjectIterator ObjectIterator = TouhouEngine::GetActiveObjectsIterator();
				while (ObjectIterator.HasEnded() == false)
				{
					if (ObjectIterator->GetTag() == "Enemy_Bullet")
					{
						if (Position.DistanceToPoint(ObjectIterator->Position) <= TeleportBulletDestructionRadius)
						{
							//nu ska vi förstöra föremålet
							TouhouEngine::Destroy(*ObjectIterator);
						}
					}
					ObjectIterator++;
				}
				//for (int i = 0; i < TouhouEngine::ActiveGameObjects.size(); i++)
				//{
				//	if (TouhouEngine::ActiveGameObjects[i]->GetTag() == "Enemy_Bullet")
				//	{
				//		if (Position.DistanceToPoint(TouhouEngine::ActiveGameObjects[i]->Position) <= TeleportBulletDestructionRadius)
				//		{
				//			//nu ska vi förstöra föremålet
				//			TouhouEngine::Destroy(TouhouEngine::ActiveGameObjects[i]);
				//		}
				//	}
				//}
			}
		}
		KlickadeSpaceInnan = true;
	}
	else
	{
		KlickadeSpaceInnan = false;
	}
}