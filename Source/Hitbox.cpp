#include <Hitbox.h>
#include <MinMath.h>
#include <MBUtility/MBMath.h>
#include<iostream>
/*
bool Hitbox_Rectangle::Rectangle_Collision(Hitbox_Rectangle a,Hitbox_Rectangle b)
{

}
*/
struct Olikhet
{
	float Koeffecient;
	float Konstant;
	bool ParralelMedYAxeln;
	bool SkaVaraSt�rre�n;
};
class OlikheterAvRektangel
{
private:
	Olikhet Olikheter[4];
public:
	bool PointInRectangle(Vector2D Point)
	{
		//det h�r borde bli ganska straightforward med denn struktur
		//vi kollar om dn uppfyller kraven f�r alla vora olikheter
		bool UppfyllerAllaOlikheter = true;
		for (int i = 0; i < 4; i++)
		{
			if (Olikheter[i].ParralelMedYAxeln)
			{
				if (Olikheter[i].SkaVaraSt�rre�n)
				{
					//nu kollar vi bara x axeln
					//h�r ska den vara mindre f�r att inte st�mma
					if (Point.x < Olikheter[i].Konstant)
					{
						UppfyllerAllaOlikheter = false;
						break;
					}
				}
				else
				{
					if (Point.x > Olikheter[i].Konstant)
					{
						UppfyllerAllaOlikheter = false;
						break;
					}
				}
			}
			else
			{
				if (Olikheter[i].SkaVaraSt�rre�n)
				{
					//�r dne mindre s� �r den ju inte i
					if (Olikheter[i].Koeffecient*Point.x + Olikheter[i].Konstant > Point.y)
					{
						UppfyllerAllaOlikheter = false;
						break;
					}
				}
				else
				{
					if (Olikheter[i].Koeffecient * Point.x + Olikheter[i].Konstant < Point.y)
					{
						UppfyllerAllaOlikheter = false;
						break;
					}
				}
			}
		}
		return(UppfyllerAllaOlikheter);
	}
	OlikheterAvRektangel(Vector2D Position, Vector2D Hitboxen, float Rotation)
	{
		//kod som faktiskt skapar olikheterna
		if(Math::FMod(Rotation,90) == 0)
		{
			//nu kommer det existera saker som �r parallella med y axeln, vilket vi inte gillar
			//d�rf�r s�tter vi parallel flagen h�r och lutningen h�r d� vi vet att det blir undantagsfall 
			/*
			float RotationUnder360 = Math::FMod(Rotation, 360);
			Olikheter[0].Koeffecient = 0;
			Olikheter[1].Koeffecient = 0;
			Olikheter[2].Koeffecient = 0;
			Olikheter[3].Koeffecient = 0;
			if (RotationUnder360 == 90 || RotationUnder360 == 270)
			{
				Olikheter[0].ParralelMedYAxeln = true;
				Olikheter[1].ParralelMedYAxeln = false;
				Olikheter[2].ParralelMedYAxeln = true;
				Olikheter[3].ParralelMedYAxeln = false;

			}
			else
			{
				Olikheter[0].ParralelMedYAxeln = false;
				Olikheter[1].ParralelMedYAxeln = true;
				Olikheter[2].ParralelMedYAxeln = false;
				Olikheter[3].ParralelMedYAxeln = true;
			}
			//vi beh�ver �ven fixa deras koeffecient
			float Sign13 = Math::Sign(Math::Sin(Rotation + 90));
			float Sign24;
			if (RotationUnder360 == 0 || RotationUnder360 == 90)
			{
				Sign24 = 1;
			}
			else
			{
				Sign24 = -1;
			}
			Olikheter[0].Konstant = Sign13 * (Hitboxen.y / 2) + Position.y;
			Olikheter[1].Konstant = -Sign24 * (Hitboxen.x / 2) + Position.y;
			Olikheter[2].Konstant = -Sign13 * (Hitboxen.y / 2) + Position.y;
			Olikheter[3].Konstant = Sign24 * (Hitboxen.x / 2) + Position.y;
			*/

			float RotationUnder360 = Math::FMod(Rotation, 360);
			//alternativ, vi p� samma s�tt som under deassocierar dem med deras "�kta" position och kollar enbart vilken som �r parallel med y axeln
			bool Linje13Parallel = true;
			if (RotationUnder360 == 0 || RotationUnder360 == 180 || RotationUnder360 == 360)
			{
				//nu �r det inte parallel
				Linje13Parallel = false;
			}
			//nu ska vi ta och g�ra s� att motsvarande grejer �r parallela
			if (Linje13Parallel)
			{
				Olikheter[0].ParralelMedYAxeln = true;
				Olikheter[1].ParralelMedYAxeln = false;
				Olikheter[2].ParralelMedYAxeln = true;
				Olikheter[3].ParralelMedYAxeln = false;
			}
			else
			{
				Olikheter[0].ParralelMedYAxeln = false;
				Olikheter[1].ParralelMedYAxeln = true;
				Olikheter[2].ParralelMedYAxeln = false;
				Olikheter[3].ParralelMedYAxeln = true;
			}
			//att den �r parallel g�r s� att man sj�lvklart j�mf�r med x
			//nu g�r vi barta s� att en av v�ra olikheter �r positiv och den andra negativ
			Olikheter[0].Konstant = Hitboxen.y / 2;
			Olikheter[0].SkaVaraSt�rre�n = false;

			Olikheter[1].Konstant = -Hitboxen.x / 2;
			Olikheter[1].SkaVaraSt�rre�n = true;

			Olikheter[2].Konstant = -Hitboxen.y / 2;
			Olikheter[2].SkaVaraSt�rre�n = true;

			Olikheter[3].Konstant = Hitboxen.x / 2;
			Olikheter[3].SkaVaraSt�rre�n = false;
			//eftersom dem som �r parallela med y axeln ska j�mf�ra med x och den andra y s� adderar vi motsvarande koordinat beroende p�
			for (int i = 0; i < 4;i++)
			{
				if (Olikheter[i].ParralelMedYAxeln)
				{
					Olikheter[i].Konstant += Position.x;
				}
				else
				{
					Olikheter[i].Konstant += Position.y;
				}
			}
			//m�ste ju ocks� s�tta deras lutning
			Olikheter[0].Koeffecient = 0;
			Olikheter[1].Koeffecient = 0;
			Olikheter[2].Koeffecient = 0;
			Olikheter[3].Koeffecient = 0;
		}
		else
		{
			//nu vet vi att dem inte �r parallela med y axeln, och vi kan d�rmed generellt fixa utr�kningarna
			for (int i = 0; i < 4; i++)
			{
				Olikheter[i].ParralelMedYAxeln = false;
			}
			float LutningF�r13 = std::tan(MBMath::DegreeToRadian(Rotation)); 
			float LutningF�r24 = (-1) / LutningF�r13;
			//avst�nd formeln f�r en linje p� formen ax +by + c = 0 och en punkt (q1,q2) -> |aq1+bq2+c|/(sqrt(a^2+b^2)) 
			//d� vi vill att avst�ndet ska vara Hitboxen.(y/x)/2 s� f�r vi att givet en lutning �r v�r ekvation lutning*x-y+c.
			//flr att sedan avst�ndet till position ska vara lika med det vill vi att hitbox.x/2 = |lutning*position.x-position.y+c|/(sqrt(lutning^2 +1))
			//d� vi vill l�sa f�r c f�r vi
			//-> hitbox.x/2 * sqrt(lutning^2+1) = |lutning*position.x-position.y+c|
			//vi kan nu antingen anta att h�ger ledet �r negativt, under punkten, eller positivt, �ver punkten, vilket �ndrar vad c �r. 
			//�r det det st�rra v�rdet p� c �r linjen �ver, ananrs �r den under
			//eftersom rektangeln �r likformig beh�ver vi bara r�kn ut 1 c och se vilken den ska passa till, och ta motsatsen till den andra
			//vi antar att h�gerledet �r positivt och d� �r |lutning*position.x-position.y+c| = lutning*position.x-position.y+c
			//vi f�r d� slutligen c = (avst�ndet) * sqrt(lutning^2+1) - lutning*position.x + position.y
			//om det slutligen �r negativt f�r vi att -(avst�nd) * sqrt(lutning^2+1) = lutning*position.x-position.y+c
			//-> c = (-(avst�nd) * sqrt(lutning^2+1)) - lutning*position.x+position.y
			float PositivtCF�r13 = (Hitboxen.y / 2) * std::sqrt(LutningF�r13 * LutningF�r13 + 1) - LutningF�r13 * Position.x + Position.y;
			float NegativtCF�r13 = (-(Hitboxen.y / 2) * std::sqrt(LutningF�r13 * LutningF�r13 + 1)) - LutningF�r13 * Position.x + Position.y;
			float PositivtCF�r24 = (Hitboxen.x / 2) * std::sqrt(LutningF�r24 * LutningF�r24 + 1) - LutningF�r24 * Position.x + Position.y;
			float NegativtCF�r24 = (-(Hitboxen.x / 2) * std::sqrt(LutningF�r24 * LutningF�r24 + 1)) - LutningF�r24 * Position.x + Position.y;
			//nu f�rst s�tter vi lutningen f�r alla
			Olikheter[0].Koeffecient = LutningF�r13;
			Olikheter[1].Koeffecient = LutningF�r24;
			Olikheter[2].Koeffecient = LutningF�r13;
			Olikheter[3].Koeffecient = LutningF�r24;
			//till skillnad fr�n v� f�rra kod s� var vi tvugna att alltid d�r avg�ra om vi skulle vara st�rra eller mindre, men h�r kan vi ist�llet g�ra i sj�lva olikheten
			//vi vet ocks� att varje linje har en som �r parralel f�rskjuten, men framf�rallt, vet vi att den med det st�rre c v�rdet ska vi alltid vara under
			//d�rf�r de associerar vi nu varje olikhet med var den faktiskt komm ifr�n
			Olikheter[0].Konstant = PositivtCF�r13;
			Olikheter[0].SkaVaraSt�rre�n = false;

			Olikheter[1].Konstant = PositivtCF�r24;
			Olikheter[1].SkaVaraSt�rre�n = false;

			Olikheter[2].Konstant = NegativtCF�r13;
			Olikheter[2].SkaVaraSt�rre�n = true;

			Olikheter[3].Konstant = NegativtCF�r24;
			Olikheter[3].SkaVaraSt�rre�n = true;
		}
		//nu printar vi olikheterna f�r debug syften
		for (int i = 0; i < 4; i++)
		{
			Olikhet Olikheten = Olikheter[i];
			//std::cout << Olikheten.Koeffecient << " " << Olikheten.Konstant << " " << Olikheten.ParralelMedYAxeln << " " << Olikheten.SkaVaraSt�rre�n << std::endl;
		}
	}
	bool RektanglarSk�rIvarandra(OlikheterAvRektangel AndraRektangeln)
	{
		//f�r varje h�rn i denna rektangel kollar vi om den sk�r i den andra rektangeln och sedan d� huruvida denn punkt �r i omkretsen
		bool Sk�rVarandraIOmkretsen = false;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				//sk�rnings punkten
				//ax + b = cx + d
				//-> x = (d-b)/(a-c)
				Vector2D Sk�rningspunkt = Vector2D(0, 0);
				if (Olikheter[i].ParralelMedYAxeln && !AndraRektangeln.Olikheter[j].ParralelMedYAxeln)
				{
					//bara v�nstra parallel
					Sk�rningspunkt.x = Olikheter[i].Konstant;
					Sk�rningspunkt.y = AndraRektangeln.Olikheter[j].Koeffecient * Sk�rningspunkt.x + AndraRektangeln.Olikheter[j].Konstant;
				}
				if (!Olikheter[i].ParralelMedYAxeln && AndraRektangeln.Olikheter[j].ParralelMedYAxeln)
				{
					//h�gre �r parallel
					Sk�rningspunkt.x = AndraRektangeln.Olikheter[j].Konstant;
					Sk�rningspunkt.y = Olikheter[i].Koeffecient * Sk�rningspunkt.x + Olikheter[i].Konstant;
				}
				if (Olikheter[i].ParralelMedYAxeln && AndraRektangeln.Olikheter[j].ParralelMedYAxeln)
				{
					//b�da �r paralellea, s� vi tar och kollar om deras konstant �r samma, annars s� bara skippar vi
					if (Olikheter[i].Konstant == AndraRektangeln.Olikheter[j].Konstant)
					{
						continue;
					}
					else
					{
						//inbgen sk�rningspunkt att j�mflra
						continue;
					}
				}
				if (!Olikheter[i].ParralelMedYAxeln && !AndraRektangeln.Olikheter[j].ParralelMedYAxeln)
				{
					//dem �r inte parallela med y, f�rst kollar vi om dem �r paralella
					if ((Olikheter[i].Koeffecient != AndraRektangeln.Olikheter[j].Koeffecient) || Olikheter[i].Konstant == AndraRektangeln.Olikheter[j].Konstant)
					{
						//det existerar en sk�rningspunkt
						//sk�rnings punkten
						//ax + b = cx + d
						//-> x = (d-b)/(a-c)
						//om deras konstant �r samma och dem �r paralella continuar vi
						if (Olikheter[i].Koeffecient == AndraRektangeln.Olikheter[j].Koeffecient)
						{
							//detta �r ett ganska intressant specialfall, eftersom dem enbart kan �verlappa om en av punkterna �r i den andra
							continue;
						}
						Sk�rningspunkt.x = (AndraRektangeln.Olikheter[j].Konstant - Olikheter[i].Konstant) / (Olikheter[i].Koeffecient - AndraRektangeln.Olikheter[j].Koeffecient);
						Sk�rningspunkt.y = Sk�rningspunkt.x * Olikheter[i].Koeffecient + Olikheter[i].Konstant;
					}
					else
					{
						//exister inte en sk�rningspunkt
						continue;
					}
				}
				//nu kollar vi huruvida skr�ningspunkten sk�r
				if (PointInRectangle(Sk�rningspunkt))
				{
					//m�ste �ven vara i den andra rektangelns omkrets
					if (AndraRektangeln.PointInRectangle(Sk�rningspunkt))
					{
						Sk�rVarandraIOmkretsen = true;
						//std::cout << Sk�rningspunkt.x << " " << Sk�rningspunkt.y << std::endl;
						break;
					}
				}
			}
			if (Sk�rVarandraIOmkretsen)
			{
				break;
			}
		}
		return(Sk�rVarandraIOmkretsen);
	}
};
bool CollisionClass::Rectangle_Collision(Vector2D a_position,Vector2D a_Hitbox, float a_rotation, Vector2D b_position, Vector2D b_Hitbox, float b_rotation)
{
	/*
		    1
		----------
		|		 |
		|		 |
	2	|		 |  4
		|		 |
		----------
			3
		2		  1
		----------
		|		 |
		|		 |
		|		 |
		|		 |
		----------
		3	      4
	*/
	//funktionen best�r av en x komponent som �r [0] och en konstant som �r [1]
	OlikheterAvRektangel AOlikheter = OlikheterAvRektangel(a_position, a_Hitbox, a_rotation);
	OlikheterAvRektangel BOlikheter = OlikheterAvRektangel(b_position, b_Hitbox, b_rotation);

	std::vector<Vector2D> a_Vertexes = std::vector<Vector2D>(4);
	std::vector<Vector2D> b_Vertexes = std::vector<Vector2D>(4);
	
	
	//nu vill vi �ven assigna punkterna v�rden
	float a_Diagonal = Math::sqrt(a_Hitbox.x * a_Hitbox.x + a_Hitbox.y * a_Hitbox.y);
	float b_Diagonal = Math::sqrt(b_Hitbox.x * b_Hitbox.x + b_Hitbox.y * b_Hitbox.y);
	for (int i = 0; i < a_Vertexes.size();i++)
	{
		//skillnaden fr�n mittpunkten �r diagonal som hypotenusa, med vinkel 45 + 90*i
		//f�rst beh�ver vi den vinkeln, r'knar ut det ovan s� vi optimiserar lite tid
		
		a_Vertexes[i].x = a_position.x + std::cos(MBMath::DegreeToRadian(a_rotation+45+90*i)) * a_Diagonal;
		a_Vertexes[i].y = a_position.y + std::sin(MBMath::DegreeToRadian(a_rotation+45+90*i)) * a_Diagonal;

		b_Vertexes[i].x = b_position.x + std::cos(MBMath::DegreeToRadian(b_rotation + 45 + 90 * i)) * b_Diagonal;
		b_Vertexes[i].y = b_position.y + std::sin(MBMath::DegreeToRadian(b_rotation + 45 + 90 * i)) * b_Diagonal;
	}
	//nu kollar vi om dem kolliderar
	//Finns 2 saker vi beh�ver kolla:
	//1: om en punkt �r den andras area
	//2: om n�gon av kanterna sk�r varandra inom det valda avst�ndet
	//vi strukturerar det som en while loop som breakas n�r n�gon av dem �r sanna. D� det �r mycket mer troligt att 1 av  punkterna �r i den andras omkrets s� kollar vi det f�rst f�r b�st prestanda
	bool Collides = false;
	//kollar varje h�rn f�r varje kvadrat
	for (int i = 0; i < 4; i++)
	{
		if (BOlikheter.PointInRectangle(a_Vertexes[i]))
		{
			//en av as punkter �r i b, ergo vi kolliderar
			Collides = true;
			break;
		}
		if (AOlikheter.PointInRectangle(b_Vertexes[i]))
		{
			//en av as punkter �r i b, ergo vi kolliderar
			Collides = true;
			break;
		}
	}
	if (Collides)
	{
		//std::cout << "Punkt �r i den andres omkrets" << std::endl;
	}
	//om vi inte redan kolliderat har vi specialfallat som g�r att vi m�ste kolla huruvida n�gon av kanterna sk�rvarandra
	if (!Collides)
	{
		//nu kollar vi om den sker med i omkretesen
		if (AOlikheter.RektanglarSk�rIvarandra(BOlikheter))
		{
			Collides = true;
			//std::cout << "Rektanglarnas sidor skar i varandra och ingen punkt var i deras omkrets" << std::endl;
		}
	}
	return(Collides);
}











/*
if (j == 0 || j == 2)
{
	//det h�'r borde vi egentligen modula med 360, men f�r fixa det i mitt library f�rst
	if (a_rotation < 90 && a_rotation > 270)
	{
		// nu ska v�r punkt vara mindre �n den f�rsta olikheten och st�rre �n andra
		if (j == 0)
		{
			if (a_Vertexes[i].y <= (a_Funktioner[j][0] * a_Vertexes[i].x + a_Funktioner[j][1]))
			{
				AUppfyllerAlla = false;
			}
		}
		if (j == 2)
		{
			if (a_Vertexes[i].y >= (a_Funktioner[j][0] * a_Vertexes[i].x + a_Funktioner[j][1]))
			{
				AUppfyllerAlla = false;
			}
		}
	}
	else
	{
		// nu ska v�r punkt vara st�rre �n den f�rsta olikheten och mindre �n andra

	}
}
else
{
	if (true)
	{
		// nu ska v�r punkt vara mindre �n den f�rsta olikheten och st�rre �n andra

	}
}
*/
/**/