#include <MainMenu.h>
#include <Engine\TextRendering.h>
#include <SpelObjekt.h>
#include <stdlib.h>
MainMenu::MainMenu()
{

}
void MainMenu::Update()
{
	//ritar dem 3 optionsen vi har
	float VerticalPadding = 2;
	std::vector<int> Layer = { 1,1,1,1 };
	int Layer2[] = { 1,1,1,1 };
	if (!InSubMenu)
	{
		DrawTextRectangle("Start Game", Vector2D(0, 2), Layer, 0.4);
		DrawTextRectangle("Tutorial", Vector2D(0, 2 - VerticalPadding), Layer, 0.4);
		DrawTextRectangle("Quit", Vector2D(0, 2 - VerticalPadding * 2), Layer, 0.4);
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
		TouhouEngine::DrawTexture("Jakob.png", Vector2D(-3, 2 - CurrentOption * VerticalPadding), 1, 1, Layer2);
		if (TouhouEngine::GetKeyPressed("enter"))
		{
			if (CurrentOption == 0)
			{
				TouhouEngine::Create(new Level1());
				TouhouEngine::Destroy(this);
			}
			if (CurrentOption == 1)
			{
				InSubMenu = true;
			}
			if (CurrentOption == 2)
			{
				//den här bara avslutar allt
				exit(0);
			}
		}
	}
	else
	{
		DrawTextRectangle("Press Z to fire. This consumes some Energy", Vector2D(0, 3), Layer, 0.2);
		DrawTextRectangle("Press C to fire a big shot. This consumes half your Max Energy", Vector2D(0, 2), Layer, 0.2);
		DrawTextRectangle("Press X to use on of your bombs. This clears the screen of enemy bullets", Vector2D(0, 1), Layer, 0.2);
		DrawTextRectangle("Press Space to Teleport. This uses half of your Max Energy", Vector2D(0, 0), Layer, 0.2);
		DrawTextRectangle("Hold shift to slow down and show your hitbox", Vector2D(0, -1), Layer, 0.2);
		DrawTextRectangle("Press Esc to return to main menu", Vector2D(0, -2), Layer, 0.3);
		if (TouhouEngine::GetKeyPressed("esc"))
		{
			InSubMenu = false;
		}
	}
	int Layer3[] = { 0,0,0,0 };
	TouhouEngine::DrawTexture("Backgrund1.png", Vector2D(0,0), 16, 9, Layer3);

}
MainMenu::~MainMenu()
{

}