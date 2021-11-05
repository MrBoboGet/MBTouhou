#pragma once
#include <Renderer.h>
#include <Vector2D.h>
struct ColorRGBA
{
	float R;
	float G;
	float B;
	float A;
};
class DrawObject
{
public:
	std::string Texturen;
	std::string DrawType = "";
	Vector2D Position;
	Vector2D StartLine;
	Vector2D EndLine;
	float Width;
	float Height;
	ColorRGBA Color;
	std::vector<int> Layer = {0,0,0,0};
	
	DrawObject(std::string a, Vector2D b, float c, float d, int Layern[]);
	DrawObject(Vector2D Start,Vector2D End ,std::vector<int> Layern);
};
class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	static void DrawTexture(std::string Texture, Vector2D Position, float Width, float Height,int Layer[]);
	static void DrawCall(DrawObject* a);
	static Texture* LoadTextureFrom(std::string Path, std::string TextureName);
	static void DrawLine(Vector2D Start, Vector2D End,std::vector<int> Layern);
	Texture(const std::string& path);
	Texture(const std::string& path,std::string Mode);
	Texture();
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};