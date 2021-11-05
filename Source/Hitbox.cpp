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
	bool SkaVaraStörreÄn;
};
class OlikheterAvRektangel
{
private:
	Olikhet Olikheter[4];
public:
	bool PointInRectangle(Vector2D Point)
	{
		//det här borde bli ganska straightforward med denn struktur
		//vi kollar om dn uppfyller kraven för alla vora olikheter
		bool UppfyllerAllaOlikheter = true;
		for (int i = 0; i < 4; i++)
		{
			if (Olikheter[i].ParralelMedYAxeln)
			{
				if (Olikheter[i].SkaVaraStörreÄn)
				{
					//nu kollar vi bara x axeln
					//här ska den vara mindre för att inte stämma
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
				if (Olikheter[i].SkaVaraStörreÄn)
				{
					//är dne mindre så är den ju inte i
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
			//nu kommer det existera saker som är parallella med y axeln, vilket vi inte gillar
			//därför sätter vi parallel flagen här och lutningen här då vi vet att det blir undantagsfall 
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
			//vi behöver även fixa deras koeffecient
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
			//alternativ, vi på samma sätt som under deassocierar dem med deras "äkta" position och kollar enbart vilken som är parallel med y axeln
			bool Linje13Parallel = true;
			if (RotationUnder360 == 0 || RotationUnder360 == 180 || RotationUnder360 == 360)
			{
				//nu är det inte parallel
				Linje13Parallel = false;
			}
			//nu ska vi ta och göra så att motsvarande grejer är parallela
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
			//att den är parallel gör så att man självklart jämför med x
			//nu gör vi barta så att en av våra olikheter är positiv och den andra negativ
			Olikheter[0].Konstant = Hitboxen.y / 2;
			Olikheter[0].SkaVaraStörreÄn = false;

			Olikheter[1].Konstant = -Hitboxen.x / 2;
			Olikheter[1].SkaVaraStörreÄn = true;

			Olikheter[2].Konstant = -Hitboxen.y / 2;
			Olikheter[2].SkaVaraStörreÄn = true;

			Olikheter[3].Konstant = Hitboxen.x / 2;
			Olikheter[3].SkaVaraStörreÄn = false;
			//eftersom dem som är parallela med y axeln ska jämföra med x och den andra y så adderar vi motsvarande koordinat beroende på
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
			//måste ju också sätta deras lutning
			Olikheter[0].Koeffecient = 0;
			Olikheter[1].Koeffecient = 0;
			Olikheter[2].Koeffecient = 0;
			Olikheter[3].Koeffecient = 0;
		}
		else
		{
			//nu vet vi att dem inte är parallela med y axeln, och vi kan därmed generellt fixa uträkningarna
			for (int i = 0; i < 4; i++)
			{
				Olikheter[i].ParralelMedYAxeln = false;
			}
			float LutningFör13 = std::tan(MBMath::DegreeToRadian(Rotation)); 
			float LutningFör24 = (-1) / LutningFör13;
			//avstånd formeln för en linje på formen ax +by + c = 0 och en punkt (q1,q2) -> |aq1+bq2+c|/(sqrt(a^2+b^2)) 
			//då vi vill att avståndet ska vara Hitboxen.(y/x)/2 så får vi att givet en lutning är vår ekvation lutning*x-y+c.
			//flr att sedan avståndet till position ska vara lika med det vill vi att hitbox.x/2 = |lutning*position.x-position.y+c|/(sqrt(lutning^2 +1))
			//då vi vill lösa för c får vi
			//-> hitbox.x/2 * sqrt(lutning^2+1) = |lutning*position.x-position.y+c|
			//vi kan nu antingen anta att höger ledet är negativt, under punkten, eller positivt, över punkten, vilket ändrar vad c är. 
			//är det det störra värdet på c är linjen över, ananrs är den under
			//eftersom rektangeln är likformig behöver vi bara räkn ut 1 c och se vilken den ska passa till, och ta motsatsen till den andra
			//vi antar att högerledet är positivt och då är |lutning*position.x-position.y+c| = lutning*position.x-position.y+c
			//vi får då slutligen c = (avståndet) * sqrt(lutning^2+1) - lutning*position.x + position.y
			//om det slutligen är negativt får vi att -(avstånd) * sqrt(lutning^2+1) = lutning*position.x-position.y+c
			//-> c = (-(avstånd) * sqrt(lutning^2+1)) - lutning*position.x+position.y
			float PositivtCFör13 = (Hitboxen.y / 2) * std::sqrt(LutningFör13 * LutningFör13 + 1) - LutningFör13 * Position.x + Position.y;
			float NegativtCFör13 = (-(Hitboxen.y / 2) * std::sqrt(LutningFör13 * LutningFör13 + 1)) - LutningFör13 * Position.x + Position.y;
			float PositivtCFör24 = (Hitboxen.x / 2) * std::sqrt(LutningFör24 * LutningFör24 + 1) - LutningFör24 * Position.x + Position.y;
			float NegativtCFör24 = (-(Hitboxen.x / 2) * std::sqrt(LutningFör24 * LutningFör24 + 1)) - LutningFör24 * Position.x + Position.y;
			//nu först sätter vi lutningen för alla
			Olikheter[0].Koeffecient = LutningFör13;
			Olikheter[1].Koeffecient = LutningFör24;
			Olikheter[2].Koeffecient = LutningFör13;
			Olikheter[3].Koeffecient = LutningFör24;
			//till skillnad från vå förra kod så var vi tvugna att alltid där avgöra om vi skulle vara störra eller mindre, men här kan vi istället göra i själva olikheten
			//vi vet också att varje linje har en som är parralel förskjuten, men framförallt, vet vi att den med det större c värdet ska vi alltid vara under
			//därför de associerar vi nu varje olikhet med var den faktiskt komm ifrån
			Olikheter[0].Konstant = PositivtCFör13;
			Olikheter[0].SkaVaraStörreÄn = false;

			Olikheter[1].Konstant = PositivtCFör24;
			Olikheter[1].SkaVaraStörreÄn = false;

			Olikheter[2].Konstant = NegativtCFör13;
			Olikheter[2].SkaVaraStörreÄn = true;

			Olikheter[3].Konstant = NegativtCFör24;
			Olikheter[3].SkaVaraStörreÄn = true;
		}
		//nu printar vi olikheterna för debug syften
		for (int i = 0; i < 4; i++)
		{
			Olikhet Olikheten = Olikheter[i];
			//std::cout << Olikheten.Koeffecient << " " << Olikheten.Konstant << " " << Olikheten.ParralelMedYAxeln << " " << Olikheten.SkaVaraStörreÄn << std::endl;
		}
	}
	bool RektanglarSkärIvarandra(OlikheterAvRektangel AndraRektangeln)
	{
		//för varje hörn i denna rektangel kollar vi om den skär i den andra rektangeln och sedan då huruvida denn punkt är i omkretsen
		bool SkärVarandraIOmkretsen = false;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				//skärnings punkten
				//ax + b = cx + d
				//-> x = (d-b)/(a-c)
				Vector2D Skärningspunkt = Vector2D(0, 0);
				if (Olikheter[i].ParralelMedYAxeln && !AndraRektangeln.Olikheter[j].ParralelMedYAxeln)
				{
					//bara vänstra parallel
					Skärningspunkt.x = Olikheter[i].Konstant;
					Skärningspunkt.y = AndraRektangeln.Olikheter[j].Koeffecient * Skärningspunkt.x + AndraRektangeln.Olikheter[j].Konstant;
				}
				if (!Olikheter[i].ParralelMedYAxeln && AndraRektangeln.Olikheter[j].ParralelMedYAxeln)
				{
					//högre är parallel
					Skärningspunkt.x = AndraRektangeln.Olikheter[j].Konstant;
					Skärningspunkt.y = Olikheter[i].Koeffecient * Skärningspunkt.x + Olikheter[i].Konstant;
				}
				if (Olikheter[i].ParralelMedYAxeln && AndraRektangeln.Olikheter[j].ParralelMedYAxeln)
				{
					//båda är paralellea, så vi tar och kollar om deras konstant är samma, annars så bara skippar vi
					if (Olikheter[i].Konstant == AndraRektangeln.Olikheter[j].Konstant)
					{
						continue;
					}
					else
					{
						//inbgen skärningspunkt att jämflra
						continue;
					}
				}
				if (!Olikheter[i].ParralelMedYAxeln && !AndraRektangeln.Olikheter[j].ParralelMedYAxeln)
				{
					//dem är inte parallela med y, först kollar vi om dem är paralella
					if ((Olikheter[i].Koeffecient != AndraRektangeln.Olikheter[j].Koeffecient) || Olikheter[i].Konstant == AndraRektangeln.Olikheter[j].Konstant)
					{
						//det existerar en skärningspunkt
						//skärnings punkten
						//ax + b = cx + d
						//-> x = (d-b)/(a-c)
						//om deras konstant är samma och dem är paralella continuar vi
						if (Olikheter[i].Koeffecient == AndraRektangeln.Olikheter[j].Koeffecient)
						{
							//detta är ett ganska intressant specialfall, eftersom dem enbart kan överlappa om en av punkterna är i den andra
							continue;
						}
						Skärningspunkt.x = (AndraRektangeln.Olikheter[j].Konstant - Olikheter[i].Konstant) / (Olikheter[i].Koeffecient - AndraRektangeln.Olikheter[j].Koeffecient);
						Skärningspunkt.y = Skärningspunkt.x * Olikheter[i].Koeffecient + Olikheter[i].Konstant;
					}
					else
					{
						//exister inte en skärningspunkt
						continue;
					}
				}
				//nu kollar vi huruvida skräningspunkten skör
				if (PointInRectangle(Skärningspunkt))
				{
					//måste även vara i den andra rektangelns omkrets
					if (AndraRektangeln.PointInRectangle(Skärningspunkt))
					{
						SkärVarandraIOmkretsen = true;
						//std::cout << Skärningspunkt.x << " " << Skärningspunkt.y << std::endl;
						break;
					}
				}
			}
			if (SkärVarandraIOmkretsen)
			{
				break;
			}
		}
		return(SkärVarandraIOmkretsen);
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
	//funktionen består av en x komponent som är [0] och en konstant som är [1]
	OlikheterAvRektangel AOlikheter = OlikheterAvRektangel(a_position, a_Hitbox, a_rotation);
	OlikheterAvRektangel BOlikheter = OlikheterAvRektangel(b_position, b_Hitbox, b_rotation);

	std::vector<Vector2D> a_Vertexes = std::vector<Vector2D>(4);
	std::vector<Vector2D> b_Vertexes = std::vector<Vector2D>(4);
	
	
	//nu vill vi även assigna punkterna värden
	float a_Diagonal = Math::sqrt(a_Hitbox.x * a_Hitbox.x + a_Hitbox.y * a_Hitbox.y);
	float b_Diagonal = Math::sqrt(b_Hitbox.x * b_Hitbox.x + b_Hitbox.y * b_Hitbox.y);
	for (int i = 0; i < a_Vertexes.size();i++)
	{
		//skillnaden från mittpunkten är diagonal som hypotenusa, med vinkel 45 + 90*i
		//först behäver vi den vinkeln, r'knar ut det ovan så vi optimiserar lite tid
		
		a_Vertexes[i].x = a_position.x + std::cos(MBMath::DegreeToRadian(a_rotation+45+90*i)) * a_Diagonal;
		a_Vertexes[i].y = a_position.y + std::sin(MBMath::DegreeToRadian(a_rotation+45+90*i)) * a_Diagonal;

		b_Vertexes[i].x = b_position.x + std::cos(MBMath::DegreeToRadian(b_rotation + 45 + 90 * i)) * b_Diagonal;
		b_Vertexes[i].y = b_position.y + std::sin(MBMath::DegreeToRadian(b_rotation + 45 + 90 * i)) * b_Diagonal;
	}
	//nu kollar vi om dem kolliderar
	//Finns 2 saker vi behäver kolla:
	//1: om en punkt är den andras area
	//2: om någon av kanterna skär varandra inom det valda avståndet
	//vi strukturerar det som en while loop som breakas när någon av dem är sanna. Då det är mycket mer troligt att 1 av  punkterna är i den andras omkrets så kollar vi det först för bäst prestanda
	bool Collides = false;
	//kollar varje hörn för varje kvadrat
	for (int i = 0; i < 4; i++)
	{
		if (BOlikheter.PointInRectangle(a_Vertexes[i]))
		{
			//en av as punkter är i b, ergo vi kolliderar
			Collides = true;
			break;
		}
		if (AOlikheter.PointInRectangle(b_Vertexes[i]))
		{
			//en av as punkter är i b, ergo vi kolliderar
			Collides = true;
			break;
		}
	}
	if (Collides)
	{
		//std::cout << "Punkt är i den andres omkrets" << std::endl;
	}
	//om vi inte redan kolliderat har vi specialfallat som gör att vi måste kolla huruvida någon av kanterna skärvarandra
	if (!Collides)
	{
		//nu kollar vi om den sker med i omkretesen
		if (AOlikheter.RektanglarSkärIvarandra(BOlikheter))
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
	//det hä'r borde vi egentligen modula med 360, men får fixa det i mitt library först
	if (a_rotation < 90 && a_rotation > 270)
	{
		// nu ska vår punkt vara mindre än den första olikheten och större än andra
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
		// nu ska vår punkt vara större än den första olikheten och mindre än andra

	}
}
else
{
	if (true)
	{
		// nu ska vår punkt vara mindre än den första olikheten och större än andra

	}
}
*/
/**/