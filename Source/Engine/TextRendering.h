#pragma once
#include <string>
#include <Vector2D.h>
#include <vector>
void DrawTextRectangle(std::string Text, Vector2D UpperLeftCoordinate, Vector2D LowerRightCoordinate, std::vector<int> Layer, float Size = 0.2);
void DrawTextRectangle(std::string Text, Vector2D TextCenter, std::vector<int> Layer,float Size = 0.2, float R = 0, float G = 0, float B = 0, float A = 0);