#include <MBTouhouTypes.h>
#include <Hitbox.h>

void MBTouhouEnemy_HP::Update()
{
	DrawHealthbar();
}
void MBTouhouEnemy_HP::DrawHealthbar()
{
	float LifeWidth = 0.5;
	float LifeHeight = 0.125f;
	std::array<int, 4> LifeLayer = { 0,0,0,0 };
	LifeLayer = GetGameObject()->GetComponent<SpriteRenderer>()->Layer;
	//for (int i = 0; i < 4; i++)
	//{
	//	LifeLayer[i] = this->Renderer.Layer[i];
	//}
	LifeLayer[1] += 1;

	float HealtbarPercentage = HP / MaxHp;
	TouhouEngine::DrawTexture("Green.png", Vector2D(GetGameObject()->Position.x - (LifeWidth / 2 - (LifeWidth / 2) * HealtbarPercentage), GetGameObject()->Position.y + 0.5f + HealthBarOffset), LifeWidth * HealtbarPercentage, LifeHeight, LifeLayer);
	TouhouEngine::DrawTexture("RedSquare.png", Vector2D(GetGameObject()->Position.x + (LifeWidth / 2 - (LifeWidth / 2) * (1 - HealtbarPercentage)), GetGameObject()->Position.y + 0.5f + HealthBarOffset), LifeWidth * (1 - HealtbarPercentage), LifeHeight, LifeLayer);
}
MBTouhouRegularEnemy::MBTouhouRegularEnemy()
{
	AddComponent(new SpriteRenderer());
	AddComponent(new Rectangle_Hitbox());
	AddComponent(new MBTouhouEnemy_HP());
	//AddComponent()
}