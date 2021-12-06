#pragma once
#include <string>
#include <Vector2D.h>
#include <vector>
#include <array>
#include <Engine.h>
#include <MBGameEngine/MBGameEngine.h>
void DrawTextRectangle(MBGameEngine::MBGameEngine& Engine, std::string Text, Vector2D UpperLeftCoordinate, Vector2D LowerRightCoordinate, std::array<int, 4> Layer, float Size = 0.2);
void DrawTextRectangle(MBGameEngine::MBGameEngine& Engine,std::string Text, Vector2D TextCenter, std::array<int, 4> Layer,float Size = 0.2, float R = 0, float G = 0, float B = 0, float A = 0);