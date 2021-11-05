#pragma once
class Vector2D
{
public:
	float x;
	float y;
	Vector2D(float a, float b);
	float Length();
	void Rotate(float Degrees);
	Vector2D Normalized();
	float DistanceToPoint(Vector2D Point);
	static float DistanceToPoint(Vector2D a, Vector2D b);
	static float AngleFromXAxis(Vector2D a);
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
	~Vector2D();
	Vector2D();
};