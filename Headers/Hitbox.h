#pragma once
#include <vector>
#include <Vector2D.h>
class Hitbox_Rectangle
{
	float Rectangle[4][2] = { {0,0},{0,0},{0,0},{0,0} };
	bool Rectangle_Collision(Hitbox_Rectangle a, Hitbox_Rectangle b);
};
class CollisionClass
{
public:
	static bool Rectangle_Collision(Vector2D a_position, Vector2D a_Hitbox, float a_rotation, Vector2D b_position, Vector2D b_hitbox, float b_rotation);
};