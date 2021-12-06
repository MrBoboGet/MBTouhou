#pragma once
#include <Engine.h>
constexpr float MBTOUHOU_SCREEN_HEIGHT = 9;
constexpr float MBTOUHOU_SCREEN_WIDTH = 16;

class MBTouhou_Bullet_DeleteOffScreen : public Component
{
private:
	float m_ScreenHeight = MBTOUHOU_SCREEN_HEIGHT;
	float m_ScreenWidth = MBTOUHOU_SCREEN_WIDTH;
	float m_Margin = 2;
	MBGameEngine::ObjectReference<SpriteRenderer> m_ObjectRenderer;
public:
	void Update() override;
};

class TouhouPlayer_HP : public Component
{
private:
	bool m_GotHit = false;
	int m_Invincible_Timer = 0;
	int m_TotalHP = 0;
public:
	TouhouPlayer_HP(int MaxHP)
	{
		m_TotalHP = MaxHP;
	}
	void RegisterCollision();
	void Update() override;
};
class MBTouhouEnemy_HP : public Component
{
protected:
	void DrawHealthbar();
public:
	int MaxHP = 0;
	int HP = 0;
	int HealthBarOffset = 0;
	void Update() override;
};
class MBTouhouRegularEnemy_DeleteOffScreen : public Component
{
private:
	float m_ScreenHeight = MBTOUHOU_SCREEN_HEIGHT;
	float m_ScreenWidth = MBTOUHOU_SCREEN_WIDTH;
	float m_Margin = 2;
	MBGameEngine::ObjectReference<SpriteRenderer> m_ObjectRenderer;
public:
	//MBTouhouRegularEnemy();
	void Update() override;
};
class MBTouhouRegularEnemy : public GameObject
{
private:

public:
	MBTouhouRegularEnemy();
};