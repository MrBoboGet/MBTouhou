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

				//vi tpar oss beroende p� vilka knappar vi h�ller in
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
				//vi teleporterar oss str�ckan
				Position = Vector2D(Position.x + Direction.x * TeleportDistance, Position.y + Direction.y * TeleportDistance);
				//nu f�rst�r vi kulor i en viss radie
				//detta �r verkligen inte sj�lvklart, men det vi g�r nu �r att vi loopar igenom alla objekt, kollar om dem �r av en enemy kula, och �r den n�ra nog f�rst�r vi den
				ActiveObjectIterator ObjectIterator = TouhouEngine::GetActiveObjectsIterator();
				while (ObjectIterator.HasEnded() == false)
				{
					if (ObjectIterator->GetTag() == "Enemy_Bullet")
					{
						if (Position.DistanceToPoint(ObjectIterator->Position) <= TeleportBulletDestructionRadius)
						{
							//nu ska vi f�rst�ra f�rem�let
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
				//			//nu ska vi f�rst�ra f�rem�let
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