#include <cmath>
class Vector2D
{
public:
	float x;
	float y;
	Vector2D(float a, float b)
	{
		x = a;
		y = b;
	}
	float Length()
	{
		return(std::sqrt(x * x + y * y));
	}
	friend Vector2D operator+(Vector2D a, Vector2D b)
	{
		return(Vector2D(a.x + b.x, a.y + b.y));
	}
	friend Vector2D operator-(Vector2D a, Vector2D b)
	{
		return(Vector2D(a.x - b.x, a.y - b.y));
	}
	friend Vector2D operator* (Vector2D a, float b) {
		return(Vector2D(a.x * b, a.y * b));
	}
	friend Vector2D operator* (float a, Vector2D b) {
		return(b * a);
	}
};
class GameObject
{
public:
	Vector2D Position;
	float Rotation;
	//någon form av rendering funktion, eller är det något vi kan lägga till efter? 
	//man skulle kunna göra update och liknande virtual, och ha grundläggande grejer i dem så att man alltid vet att dem har en
};
