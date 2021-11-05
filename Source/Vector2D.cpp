#include <math.h>
#include <MinMath.h>
#include <iostream>
#include <Vector2D.h>
#include <MBUtility/MBMath.h>
Vector2D::Vector2D(float a, float b)
{
	x = a;
	y = b;
}
float Vector2D::Length()
{
	return(sqrt(x * x + y * y));
}
Vector2D::Vector2D()
{
	x = 0;
	y = 0;
}
Vector2D::~Vector2D()
{

}
void Vector2D::Rotate(float Degrees)
{
	auto TempX = x;
	auto TempY = y;
	x = TempX * std::cos(MBMath::DegreeToRadian(Degrees)) - TempY * std::sin(MBMath::DegreeToRadian(Degrees));
	y = TempX * std::sin(MBMath::DegreeToRadian(Degrees)) + TempY * std::cos(MBMath::DegreeToRadian(Degrees));
}
Vector2D Vector2D::Normalized()
{
	//returnerar vektorn fast med l�ngd 1
	//vi returnerar delarna delat med C
	float C = sqrt(x * x + y * y);
	return(Vector2D(x / C, y / C));
}
float Vector2D::DistanceToPoint(Vector2D Point)
{
	return(sqrt((x - Point.x) * (x - Point.x) + (y - Point.y) * (y - Point.y)));
}
float Vector2D::DistanceToPoint(Vector2D a, Vector2D b)
{
	return(sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
}
float Vector2D::AngleFromXAxis(Vector2D a)
{
	//alla arc vinkel funktioner ger ju alltid dubbletter av v�rden, men dem mest intuitiva f�r dessa �r Cosiuns. Ger dubbletter p� v�rden efter 180. Vi tar allts� arccos av vinkel
	//och om y sedan �r negativt l�gger vi till 180-vinkeln
	//TODO implementera en egen arccos funktion
	a = a.Normalized();
	float ArcCosIGrader = acos(a.x)*(180/3.1415926535);
	std::cout << ArcCosIGrader << std::endl;
	if (a.y < 0)
	{
		ArcCosIGrader = (360 - ArcCosIGrader);
	}
	std::cout << ArcCosIGrader << std::endl;
	return(ArcCosIGrader);
}