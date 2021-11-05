#include <PausMenu.h>
#include <Engine\TextRendering.h>
#include <MainMenu.h>
#include <SpelObjekt.h>
PausMenu::PausMenu()
{
	Name = "";
	Tag = "PausMenu";
}
void PausMenu::Update()
{
	if (!Paused)
	{
		if (TouhouEngine::GetKeyPressed("esc"))
		{
			Paused = true;
			for (int i = 0; i < TouhouEngine::ActiveGameObjects.size(); i++)
			{
				if (TouhouEngine::ActiveGameObjects[i]->GetTag() != "PausMenu")
				{
					TouhouEngine::ActiveGameObjects[i]->Active = false;
				}
			}
		}
	}
	else
	{
		if (TouhouEngine::GetKeyPressed("esc"))
		{
			Paused = false;
			for (int i = 0; i < TouhouEngine::ActiveGameObjects.size(); i++)
			{
				TouhouEngine::ActiveGameObjects[i]->Active = true;
			}
		}
		else
		{
			int Layer3[] = { 1000000,0,0,0 };
			std::vector<int> Layer4545 = { 1000001,0,0,0 };
			Texture::DrawTexture("Backgrund1.png", Vector2D(0, 0), 16, 9, Layer3);
			DrawTextRectangle("Continue", Vector2D(0, 1.5), Layer4545, 0.4);
			DrawTextRectangle("Quit", Vector2D(0, -1.5), Layer4545, 0.4);
			DrawTextRectangle("Retry", Vector2D(0, 0), Layer4545, 0.4);
			if (TouhouEngine::GetKeyPressed("down") || TouhouEngine::GetKeyPressed("s"))
			{
				CurrentOption += 1;
				CurrentOption %= NumberOfOptions;
			}
			if (TouhouEngine::GetKeyPressed("up") || TouhouEngine::GetKeyPressed("w"))
			{
				CurrentOption -= 1;
				if (CurrentOption < 0)
				{
					CurrentOption = NumberOfOptions - 1;
				}
			}
			Texture::DrawTexture("Jakob.png", Vector2D(-3, 1.5 - 1.5*CurrentOption), 1, 1, Layer3);
			if (TouhouEngine::GetKeyPressed("enter"))
			{
				if (CurrentOption == 0)
				{
					Paused = false;
					for (int i = 0; i < TouhouEngine::ActiveGameObjects.size(); i++)
					{
						TouhouEngine::ActiveGameObjects[i]->Active = true;
					}
				}
				if (CurrentOption == 1)
				{
					std::vector<GameObject*> ObjectAttDelete = TouhouEngine::ActiveGameObjects;
					for (int i = 0; i < ObjectAttDelete.size(); i++)
					{
						TouhouEngine::Destroy(ObjectAttDelete[i]);
					}
					TouhouEngine::Create(new Level1());
				}
				if(CurrentOption == 2)
				{
					std::vector<GameObject*> ObjectAttDelete = TouhouEngine::ActiveGameObjects;
					for (int i = 0; i < ObjectAttDelete.size(); i++)
					{
						TouhouEngine::Destroy(ObjectAttDelete[i]);
					}
					TouhouEngine::Create(new MainMenu());
				}
			}
		}
	}
}

PausMenu::~PausMenu()
{

}