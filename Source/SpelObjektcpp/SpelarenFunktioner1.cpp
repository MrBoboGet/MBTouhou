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

				//vi tpar oss beroende p? vilka knappar vi h?ller in
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
				//vi teleporterar oss str?ckan
				Vector2D Position = Transform.GetPosition();
				Position = Vector2D(Position.x + Direction.x * TeleportDistance, Position.y + Direction.y * TeleportDistance);
				//nu f?rst?r vi kulor i en viss radie
				//detta ?r verkligen inte sj?lvklart, men det vi g?r nu ?r att vi loopar igenom alla objekt, kollar om dem ?r av en enemy kula, och ?r den n?ra nog f?rst?r vi den
				std::vector<MBGameEngine::GameObjectReference> ObjectToDestroy = {};
				ActiveObjectIterator ObjectIterator = GetEngine().GetActiveObjectsIterator();
				while (ObjectIterator.HasEnded() == false)
				{
					if (ObjectIterator->GetTag() == "Enemy_Bullet")
					{
						if (Position.DistanceToPoint(ObjectIterator->Transform.GetPosition()) <= TeleportBulletDestructionRadius)
						{
							//nu ska vi f?rst?ra f?rem?let
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
				//			//nu ska vi f?rst?ra f?rem?let
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