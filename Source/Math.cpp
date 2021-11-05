#include <iostream>
namespace Math
{
	float Pow(float c, float exp)
	{
		return(0);
	}
	float sqrt(float a)
	{
		if (a == 0)
		{
			return(0);
		}
		float ForstaGissning = a / 2;
		for (int i = 0; i < 100; i++)
		{
			if (ForstaGissning*2 == 0)
			{
				return(0);
			}
			ForstaGissning = ForstaGissning - (ForstaGissning * ForstaGissning - a) / (2 * ForstaGissning);
		}
		return(ForstaGissning);
	}
	/*observera att den returnar 1 �ven om a �r 0*/
	int Sign(float a) 
	{
		if (a>=0)
		{
			/*
			if (a == 0)
			{
				return(0);
			}
			*/
			return(1);
		}
		else
		{
			return(-1);
		}
	}
	float Pi = 3.1415926535;
	//fick feta buggar n�r det blev �ver 360 tror jag. f�r konsekvent hetens skull s� bara g�r jag det positivt och sedan modular
	float Sin(float a) 
	{
		while (a<0)
		{
			a += 360;
		}

		while (a > 360)
		{
			a -= 360;
		}
		float Vinkel = (a / 360) * 2 * Pi;
		float SinusVardet = 0;
		float T�ljare = Vinkel;
		float N�mnare = 1;
		int fakultettal = 1;
		int Tecken = 1;
		for (int i = 1; i < 15; i++) 
		{
			SinusVardet += (T�ljare / N�mnare)*Tecken;
			T�ljare *= (Vinkel * Vinkel);
			N�mnare *= ((fakultettal + 1) * (fakultettal + 2));
			fakultettal += 2;
			Tecken *= -1;
		}
		return(SinusVardet);
	}
	float Cos(float a)
	{
		while (a < 0)
		{
			a += 360;
		}

		while (a > 360)
		{
			a -= 360;
		}
		float AbsCosVardet = sqrt(1 - (Sin(a) * Sin(a)));
		if (a>90 && a <270)
		{
			AbsCosVardet *= -1;
		}
		return(AbsCosVardet);
	}
	//TODO Fixa s� att Fmod �r definierad f�r negativa tal

	//v�l att m�rka, bara definierad f�r positiva tal
	float FMod(float a,float b)
	{
		//vi tar -1 som ett felv�rde
		if (b == 0)
		{
			return(-1);
		}
		else
		{
			if (a == 0)
			{
				return(0);
			}
			else
			{
				//inga undantag h�r
				float Kvot = a / b;
				//om denna kvot h�r ett heltal �r dem multiplat av varandra. Annars returnerar vi v�rdet
				float DetSomBlirKvarEfter = (Kvot - (int)Kvot) * b;
				return(DetSomBlirKvarEfter);
			}
		}
	}
	float Tan(float Grader)
	{
		return(tan(Grader * Pi / 180));
	}
	float Abs(float a)
	{
		if (a <= 0)
		{
			return(-a);
		}
		else
		{
			return(a);
		}
	}
}