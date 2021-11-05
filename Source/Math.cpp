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
	/*observera att den returnar 1 även om a är 0*/
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
	//fick feta buggar när det blev över 360 tror jag. för konsekvent hetens skull så bara gör jag det positivt och sedan modular
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
		float Täljare = Vinkel;
		float Nämnare = 1;
		int fakultettal = 1;
		int Tecken = 1;
		for (int i = 1; i < 15; i++) 
		{
			SinusVardet += (Täljare / Nämnare)*Tecken;
			Täljare *= (Vinkel * Vinkel);
			Nämnare *= ((fakultettal + 1) * (fakultettal + 2));
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
	//TODO Fixa så att Fmod är definierad för negativa tal

	//väl att märka, bara definierad för positiva tal
	float FMod(float a,float b)
	{
		//vi tar -1 som ett felvärde
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
				//inga undantag här
				float Kvot = a / b;
				//om denna kvot här ett heltal är dem multiplat av varandra. Annars returnerar vi värdet
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