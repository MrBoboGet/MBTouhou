#include <Engine\TextRendering.h>
#include<unordered_map>
#include <Texture.h>
#include <cctype>
#include<Engine.h> 
#include <iostream>
//vi har en statisk unordred map som inneh�ller all den text data vi vill kunna rendera
//egentligen borde denna typ av text rendering g�ras i form att atta l�sa in en kongig fil
//TODO G�r s� att textl�sandet kan g�ras med hj�lp av en extern konfig fil som inte �r i koden
//TODO G�r s� att vi kan l�sa in alla textures p� en g�ng, ist�llet som vi g�r nu och bara l�ser in det n�r det v�l komemr upp
//extern std::unordered_map<std::string, Texture*> LoadedTextures;

float TextHeight = 0.2;
float TextWidth = 0.2;
void DrawTextRectangle(std::string Text, Vector2D UpperLeftCoordinate, Vector2D LowerRightCoordinate, std::vector<int> Layer,float Size)
{
	TextHeight = Size;
	TextWidth = Size;
	Vector2D PositionToDraw = Vector2D(UpperLeftCoordinate.x+TextWidth/2,UpperLeftCoordinate.y-TextHeight/2);
	for (size_t i = 0; i < Text.length(); i++)
	{
		std::string CharacterToDraw;
		CharacterToDraw = Text[i];
		if (std::islower(CharacterToDraw[0]))
		{
			CharacterToDraw = CharacterToDraw + "_2";
		}
		if (CharacterToDraw[0] == ' ')
		{
			PositionToDraw.x += TextWidth;
			if (PositionToDraw.x + TextWidth / 2 > LowerRightCoordinate.x)
			{
				PositionToDraw.x = UpperLeftCoordinate.x + TextWidth / 2;
				PositionToDraw.y -= TextHeight;
			}
			continue;
		}
		if (TouhouEngine::LoadedTextures.find(CharacterToDraw) == TouhouEngine::LoadedTextures.end())
		{
			//ej loaded, vi m�ste f�rst ladda in den innan vi kan dra den
			if (CharacterToDraw[0] == '.')
			{
				std::cout << "." << std::endl;
				Texture* NewCharacterTexture = new Texture("Resources/TextImages/ZX_1.png", "NoFilter");
				TouhouEngine::LoadedTextures[CharacterToDraw] = NewCharacterTexture;
			}
			else
			{
				Texture* NewCharacterTexture = new Texture("Resources/TextImages/" + CharacterToDraw + ".png", "NoFilter");
				TouhouEngine::LoadedTextures[CharacterToDraw] = NewCharacterTexture;
			}
		}
		//nu har vi en character texture som vi nu kan anv�nda f�r att rita
		//Som det �r nu s� kommer vi bara att rita dem som om dem hade samma storlek
		if (PositionToDraw.y -TextHeight/2 < LowerRightCoordinate.y)
		{
			break;
		}
		int Layern[] = { Layer[0],Layer[1],Layer[2],Layer[3]};
		Texture::DrawTexture(CharacterToDraw, PositionToDraw, TextWidth, TextHeight, Layern);
		//f�rflyttar positionenen som om det vore helt uniform storlek p� texten
		PositionToDraw.x += TextWidth;
		if (PositionToDraw.x+TextWidth/2 > LowerRightCoordinate.x)
		{
			PositionToDraw.x = UpperLeftCoordinate.x + TextWidth / 2;
			PositionToDraw.y -= TextHeight;
		}
	}
}
void DrawTextRectangle(std::string Text, Vector2D TextCenter,std::vector<int> Layer,float Size, float R, float G, float B , float A)
{
	//vi b�rjar med att r�kna ut var n�gonstans vi ska b�rja, genom att ta bredden av all v�r text och r�kna ut hur l�ngt �t v�nster vi ska b�rja
	float WidthOfTextToDraw = 0;
	//vi komer overrida den globala variablen, v�ldigt fult
	float TextHeight = Size;
	float TextWidth = Size;
	for (size_t i = 0; i < Text.size(); i++)
	{
		WidthOfTextToDraw += TextWidth;
	}
	Vector2D PositionToDraw = Vector2D(TextCenter.x +TextWidth/2 - WidthOfTextToDraw/2, TextCenter.y);
	for (size_t i = 0; i < Text.length(); i++)
	{
		std::string CharacterToDraw;
		CharacterToDraw = Text[i];
		if (std::islower(CharacterToDraw[0]))
		{
			CharacterToDraw = CharacterToDraw + "_2";
		}
		if (CharacterToDraw[0] == ' ')
		{
			PositionToDraw.x += TextWidth;
			continue;
		}
		if (TouhouEngine::LoadedTextures.find(CharacterToDraw) == TouhouEngine::LoadedTextures.end())
		{
			//ej loaded, vi m�ste f�rst ladda in den innan vi kan dra den
			if (CharacterToDraw[0] == '.')
			{
				std::cout << "." << std::endl;
				Texture* NewCharacterTexture = new Texture("Resources/TextImages/ZX_1.png", "NoFilter");
				TouhouEngine::LoadedTextures[CharacterToDraw] = NewCharacterTexture;
			}
			else
			{
				Texture* NewCharacterTexture = new Texture("Resources/TextImages/" + CharacterToDraw + ".png", "NoFilter");
				TouhouEngine::LoadedTextures[CharacterToDraw] = NewCharacterTexture;
			}
		}
		//nu har vi en character texture som vi nu kan anv�nda f�r att rita
		//Som det �r nu s� kommer vi bara att rita dem som om dem hade samma storlek
		int Layern[] = { Layer[0],Layer[1],Layer[2],Layer[3] };
		Texture::DrawTexture(CharacterToDraw, PositionToDraw, TextWidth, TextHeight, Layern);
		//f�rflyttar positionenen som om det vore helt uniform storlek p� texten
		PositionToDraw.x += TextWidth;
	}
}