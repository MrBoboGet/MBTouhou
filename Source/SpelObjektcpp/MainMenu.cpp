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
	std::array<int,4> Layer = { 1,1,1,1 };
	std::array<int,4> Layer2 = { 1,1,1,1 };
	if (!InSubMenu)
	{
		DrawTextRectangle(GetEngine(),"Start Game", Vector2D(0, 2), Layer, 0.4);
		DrawTextRectangle(GetEngine(),"Tutorial", Vector2D(0, 2 - VerticalPadding), Layer, 0.4);
		DrawTextRectangle(GetEngine(),"Quit", Vector2D(0, 2 - VerticalPadding * 2), Layer, 0.4);
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
		MBGE::Transform TextureTransform;
		TextureTransform.SetPosition(Vector2D(-3, 2 - CurrentOption * VerticalPadding));
		GetEngine().DrawTexture("Jakob.png", TextureTransform, 1, 1, Layer2);
		if (GetEngine().GetKeyPressed(MBGameEngine::KeyCode::Enter))
		{
			if (CurrentOption == 0)
			{
				GetEngine().Create(new Level1());
				GetEngine().Destroy(this);
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
		DrawTextRectangle(GetEngine(),"Press Z to fire. This consumes some Energy", Vector2D(0, 3), Layer, 0.2);
		DrawTextRectangle(GetEngine(),"Press C to fire a big shot. This consumes half your Max Energy", Vector2D(0, 2), Layer, 0.2);
		DrawTextRectangle(GetEngine(),"Press X to use on of your bombs. This clears the screen of enemy bullets", Vector2D(0, 1), Layer, 0.2);
		DrawTextRectangle(GetEngine(),"Press Space to Teleport. This uses half of your Max Energy", Vector2D(0, 0), Layer, 0.2);
		DrawTextRectangle(GetEngine(),"Hold shift to slow down and show your hitbox", Vector2D(0, -1), Layer, 0.2);
		DrawTextRectangle(GetEngine(),"Press Esc to return to main menu", Vector2D(0, -2), Layer, 0.3);
		if (GetEngine().GetKeyPressed(MBGameEngine::KeyCode::Esc))
		{
			InSubMenu = false;
		}
	}
	std::array<int,4> Layer3 = { 0,0,0,0 };
	MBGE::Transform TextureTransform;
	TextureTransform.SetPosition(Vector2D(0, 0));
	GetEngine().DrawTexture("Backgrund1.png", TextureTransform, 16, 9, Layer3);

}
MainMenu::~MainMenu()
{

}