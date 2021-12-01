#pragma once
#include <vector>
#include <Vector2D.h>
#include <Engine.h>
class Hitbox_Rectangle
{
	float Rectangle[4][2] = { {0,0},{0,0},{0,0},{0,0} };
	bool Rectangle_Collision(Hitbox_Rectangle a, Hitbox_Rectangle b);
};
class Rectangle_Hitbox : public Component
{
private:
	static bool Rectangle_Collision(Vector2D a_position, Vector2D a_Hitbox, float a_rotation, Vector2D b_position, Vector2D b_hitbox, float b_rotation);
public:
	float Width = 0;
	float Height = 0;
	static bool Collides(const Rectangle_Hitbox * LeftHitbox, const Rectangle_Hitbox * RightHitbox);
};