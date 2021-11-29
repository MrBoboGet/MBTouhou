#include<Texture.h>
#include <Source/bildhantering/BildHantering.h>
#include <iostream>
#include <VertexBufferLayout.h>
#include <Engine.h>
//extern std::unordered_map<std::string, Texture*> LoadedTextures;
//extern std::vector<DrawObject*> DrawCalls;

DrawObject::DrawObject(std::string a, Vector2D b, float c, float d,int Layern[]) : Texturen(a),Position(b),Width(c),Height(d)
{
	DrawType = "Texture";
	Layer[0] = Layern[0];
	Layer[1] = Layern[1];
	Layer[2] = Layern[2];
	Layer[3] = Layern[3];
	Color.R = 1;
	Color.G = 1;
	Color.B = 1;
	Color.A = 1;
};
DrawObject::DrawObject(Vector2D Start, Vector2D End, std::vector<int> Layern)
{
	DrawType = "Line";
	for (int i = 0; i < Layer.size(); i++)
	{
		if (i < Layer.size())
		{
			Layer[i] = Layern[i];
		}
		else
		{
			Layer.push_back(Layern[i]);
		}
	}
	StartLine = Start;
	EndLine = End;
}


Texture::Texture(const std::string& path)
	: m_RendererID(0),m_FilePath(path), m_LocalBuffer(nullptr),m_Width(0),m_Height(0),m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
}
Texture::Texture(const std::string& path,std::string Mode)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	if (Mode == "NoFilter")
	{
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if (m_LocalBuffer)
		{
			stbi_image_free(m_LocalBuffer);
		}
	}
	else
	{
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if (m_LocalBuffer)
		{
			stbi_image_free(m_LocalBuffer);
		}
	}
}
Texture::~Texture()
{
	//Den nuvarande l�sningen beh�ller texture objeketet i alla gameobjects, s� man inte beh�ver g�ra om all kod, men 
	//g�r s� att n�r man skapar f�rem�let ser man om texturen existerar och antingen skapar eller kp

	//std::cout <<"Vi f�rst�r texturen vid den h�r positionen "<<m_RendererID<<std::endl;
	//GLCall(glDeleteTextures(1, &m_RendererID));
}
void Texture::UnBind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
void Texture::Bind(unsigned int slot ) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	//std::cout << m_RendererID <<" "<<GL_TEXTURE0+slot << std::endl;
}
Texture::Texture()
{

}
/* innan jag fuckar runt med grejer
Texture::Texture(const std::string& path)
	: m_RendererID(0),m_FilePath(path), m_LocalBuffer(nullptr),m_Width(0),m_Height(0),m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
}
*/
void Texture::DrawTexture(std::string Texturen, Vector2D Position, float Width, float Height, int Layer[])
{
	//vi till�ter bara att man ritar loaded textures, eftersom annars blir prestandan whack om man ska l�sa in varje frame
	if (TouhouEngine::LoadedTextures.find(Texturen) != TouhouEngine::LoadedTextures.end())
	{
		//Ett problem vi f�r h�r �r att man inte riktigt kan rita grejer p� den layern man vill
		//Det finns ett antal s�tt vi skulle kunna fixa det h�r p�
		//1: Ha ett draw object som vi passar till n�gon form av drawobject lista som f�ljer samma ordning
		//Problem: Ganska s� error prone med allt som har att g�ra med object och resurser, och relativt jobbigt att uppeh�lla allt vad det inneb�r
		//2: Ha en draw call object
		//f�rdelen gentemot f�reg�ende �r att den kan vara extremt enkel, inneh�ller samma argument som funktionen skulle haft
		//3: Ist�llet g�r vi alla draw calls i n�gon form av ut�kad render funktion
		//Prblem, vi vill kunna dra saker i uodate funktionen, och d� f�r vi samma problem
		//Jag tror jag l�ser det genom f�ljande, jag �ndrar om s� att draw texture ocks� specificerar en layer, och den skapar ett "Draw call" object, som vi drar i render funktionen
		
		
		DrawObject* DrawPointern = new DrawObject(Texturen, Position, Width, Height, Layer);
		TouhouEngine::DrawCalls.push_back(DrawPointern);

		
		
		/*
		float positions[]
		{
			(Position.x - Width/2)/8 , (Position.y - Height/2)/4.5,	0.0f,0.0f,
			(Position.x + Width/2)/8 , (Position.y - Height/2)/4.5,	1.0f,0.0f,
			(Position.x + Width/2)/8 , (Position.y + Height/2)/4.5,	1.0f,1.0f,
			(Position.x - Width/2)/8 , (Position.y + Height/2)/4.5,	0.0f,1.0f
		};
		unsigned int indices[] =
		{
			0,1,2,
			2,3,0
		};
		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.push<float>(2);
		layout.push<float>(2);
		va.AddBuffer(vb, layout);
		IndexBuffer ib(indices, 6);
		ib.Bind();
		LoadedTextures[Texturen]->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		*/
	}
	else 
	{
		//std::cout << "Texturen �r inte loadad" << std::endl;
		//lite av en ass l�sning d� man m�ste ha loadat in saker, varf�r gjorde jag inte bara s�att jag laddade in den h�r, the fuck
		Texture* NyTexture = new Texture("Resources/SpelResurser/Sprites/" + Texturen);
		TouhouEngine::LoadedTextures[Texturen] = NyTexture;

		DrawObject* DrawPointern = new DrawObject(Texturen, Position, Width, Height, Layer);
		TouhouEngine::DrawCalls.push_back(DrawPointern);
	}
}
GLuint DrawLineVbo = 0;
GLuint DrawLineVao = 0;
void Texture::DrawCall(DrawObject* Objectet)
{
	//std::string Texturen, Vector2D Position, float Width, float Height, int Layer[]
	if (Objectet->DrawType == "Texture")
	{
		auto Texturen = Objectet->Texturen;
		auto Position = Objectet->Position;
		auto Width = Objectet->Width;
		auto Height = Objectet->Height;
		float positions[]
		{
			(Position.x - Width / 2) / 8 , (Position.y - Height / 2) / 4.5,	0.0f,0.0f,
			(Position.x + Width / 2) / 8 , (Position.y - Height / 2) / 4.5,	1.0f,0.0f,
			(Position.x + Width / 2) / 8 , (Position.y + Height / 2) / 4.5,	1.0f,1.0f,
			(Position.x - Width / 2) / 8 , (Position.y + Height / 2) / 4.5,	0.0f,1.0f
		};
		unsigned int indices[] =
		{
			0,1,2,
			2,3,0
		};
		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.push<float>(2);
		layout.push<float>(2);
		va.AddBuffer(vb, layout);
		IndexBuffer ib(indices, 6);
		ib.Bind();
		TouhouEngine::LoadedTextures[Texturen]->Bind();

		//ny grej, vi specificerar vilken shader vi anv�nder
		Shader* ShaderToUse = TouhouEngine::GetNamedShader("SpriteShader");
		ShaderToUse->Bind();
		ShaderToUse->SetUniform1i("u_Texture", 0);
		ShaderToUse->SetUniform4f("ColorKoef",1,1,1, 1);


		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
	}
	if (Objectet->DrawType == "Line")
	{
		//h�r ska jag f�r�ska g�ra allt fr�n grunden, s� vi kan se hur man g�r
		//TODO Implementera en kamera s� att vi inte beh�ver manuellt setta f�rh�llandet och etc
		/*
		GLfloat VericesPosition[4]
		{
			Objectet->StartLine.x/8,Objectet->StartLine.y/4.5,
			Objectet->EndLine.x/8,Objectet->EndLine.y/4.5
		};
		GLuint Vbo;
		GLuint Vao;
		GLCall(glGenVertexArrays(1, &Vao));
		GLCall(glBindVertexArray(Vao));

		GLCall(glGenBuffers(1, &Vbo));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, Vbo));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(VericesPosition), VericesPosition, GL_STATIC_DRAW));

		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
		GLCall(glEnableVertexAttribArray(0));

		GLCall(Shader* ShaderToUse = TouhouEngine::LoadedShaders["MonochromeShader"]);
		GLCall(ShaderToUse->Bind());
		ShaderToUse->SetUniform4f("u_Color", 1, 0, 0,1);

		GLCall(glDrawArrays(GL_LINES, 0, 2));

		//vi deletar allt, egentligen d�lig l�sning, skrev mest f�r att ha koll p� hur allt funkade
		GLCall(glDeleteBuffers(1, &Vbo));
		GLCall(glDeleteVertexArrays(1, &Vao));
		*/
		GLfloat VericesPosition[4]
		{
			Objectet->StartLine.x/8,Objectet->StartLine.y/4.5,
			Objectet->EndLine.x/8,Objectet->EndLine.y/4.5
		};
		if(DrawLineVao == 0)
		{
			GLCall(glGenBuffers(1, &DrawLineVbo));
			GLCall(glGenVertexArrays(1, &DrawLineVao));
			GLCall(glBindVertexArray(DrawLineVao));

			GLCall(glBindBuffer(GL_ARRAY_BUFFER, DrawLineVbo));
			GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(VericesPosition), VericesPosition, GL_STATIC_DRAW));

			GLCall(glEnableVertexAttribArray(0));
			GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
			//GLCall(glBindVertexArray(0));
		}
		else
		{
			GLCall(glBindVertexArray(DrawLineVao));
			//TODO why the fuck funmkar inte den h�r grejen att bar binda r�tt vertex array
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, DrawLineVbo));
		}

		Shader * ShaderToUse = TouhouEngine::GetNamedShader("MonochromeShader");
		GLCall(ShaderToUse->Bind());
		GLCall(ShaderToUse->SetUniform4f("u_Color", 1, 0, 0, 1));

		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(VericesPosition), VericesPosition, GL_STATIC_DRAW));

		GLCall(glDrawArrays(GL_LINES, 0, 2));
		//unbindar v�ran vertex array
		GLCall(glBindVertexArray(0));
	}
}
Texture* Texture::LoadTextureFrom(std::string Path, std::string TextureName)
{
	if (TouhouEngine::LoadedTextures.find(TextureName) != TouhouEngine::LoadedTextures.end())
	{
		return(TouhouEngine::LoadedTextures[TextureName]);
	}
	else
	{
		Texture* NyTexture = new Texture(Path + TextureName);
		TouhouEngine::LoadedTextures[TextureName] = NyTexture;
	}
}
void Texture::DrawLine(Vector2D Start, Vector2D End,std::vector<int> Layern)
{
	DrawObject* DrawPointern = new DrawObject(Start,End,Layern);
	TouhouEngine::DrawCalls.push_back(DrawPointern);
}