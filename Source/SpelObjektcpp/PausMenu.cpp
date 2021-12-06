#include <PausMenu.h>
#include <Engine\TextRendering.h>
#include <MainMenu.h>
#include <SpelObjekt.h>
PausMenu::PausMenu()
{
	SetName("");
	SetTag("PausMenu");
}
void PausMenu::Update()
{
	if (!Paused)
	{
		if (GetEngine().GetKeyPressed(MBGameEngine::KeyCode::Esc))
		{
			Paused = true;
			ActiveObjectIterator ObjectsIterator = GetEngine().GetActiveObjectsIterator();
			while (ObjectsIterator.HasEnded() == false)
			{
				if (ObjectsIterator->GetTag() != "PausMenu")
				{
					ObjectsIterator->Active = false;
				}
				ObjectsIterator++;
			}
			//for (int i = 0; i < GetEngine().ActiveGameObjects.size(); i++)
			//{
			//	if (GetEngine().ActiveGameObjects[i]->GetTag() != "PausMenu")
			//	{
			//		GetEngine().ActiveGameObjects[i]->Active = false;
			//	}
			//}
		}
	}
	else
	{
		if (GetEngine().GetKeyPressed(MBGameEngine::KeyCode::Esc))
		{
			Paused = false;
			ActiveObjectIterator ObjectsIterator = GetEngine().GetActiveObjectsIterator();
			while (ObjectsIterator.HasEnded() == false)
			{
				ObjectsIterator->Active = true;
				ObjectsIterator++;
			}
			//for (int i = 0; i < GetEngine().ActiveGameObjects.size(); i++)
			//{
			//	GetEngine().ActiveGameObjects[i]->Active = true;
			//}
		}
		else
		{
			std::array<int,4> Layer3 = { 1000000,0,0,0 };
			std::array<int,4> Layer4545 = { 1000001,0,0,0 };
			MBGE::Transform TextureTransform;
			TextureTransform.SetPosition(Vector2D(0, 0));
			GetEngine().DrawTexture("Backgrund1.png", TextureTransform, 16, 9, Layer3);
			DrawTextRectangle(GetEngine(),std::string("Continue"), Vector2D(0, 0), Vector2D(0, 0), Layer4545, 23);
			DrawTextRectangle(GetEngine(),"Continue", Vector2D(0, 1.5), Layer4545, 0.4);
			DrawTextRectangle(GetEngine(),"Quit", Vector2D(0, -1.5), Layer4545, 0.4);
			DrawTextRectangle(GetEngine(),"Retry", Vector2D(0, 0), Layer4545, 0.4);
			if (GetEngine().GetKeyPressed(MBGameEngine::KeyCode::Down) || GetEngine().GetKeyPressed(MBGameEngine::KeyCode::S))
			{
				CurrentOption += 1;
				CurrentOption %= NumberOfOptions;
			}
			if (GetEngine().GetKeyPressed(MBGameEngine::KeyCode::Up) || GetEngine().GetKeyPressed(MBGameEngine::KeyCode::W))
			{
				CurrentOption -= 1;
				if (CurrentOption < 0)
				{
					CurrentOption = NumberOfOptions - 1;
				}
			}
			//MBGE::Transform TextureTransform;
			TextureTransform.SetPosition(Vector2D(-3, 1.5 - 1.5 * CurrentOption));
			GetEngine().DrawTexture("Jakob.png", TextureTransform, 1, 1, Layer3);
			if (GetEngine().GetKeyPressed(MBGameEngine::KeyCode::Enter))
			{
				if (CurrentOption == 0)
				{
					Paused = false;
					ActiveObjectIterator ObjectsIterator = GetEngine().GetActiveObjectsIterator();
					while (ObjectsIterator.HasEnded() == false)
					{
						ObjectsIterator->Active = true;
						ObjectsIterator++;
					}
					//for (int i = 0; i < GetEngine().ActiveGameObjects.size(); i++)
					//{
					//	GetEngine().ActiveGameObjects[i]->Active = true;
					//}
				}
				if (CurrentOption == 1)
				{
					GetEngine().ClearObjects();
					GetEngine().Create(new Level1());
				}
				if(CurrentOption == 2)
				{
					GetEngine().ClearObjects();
					GetEngine().Create(new MainMenu());
				}
			}
		}
	}
}

PausMenu::~PausMenu()
{

}