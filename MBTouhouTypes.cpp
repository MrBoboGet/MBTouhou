#include <MBTouhouTypes.h>
#include <Hitbox.h>

//BEGIN TouhouPlayer_HP
void TouhouPlayer_HP::RegisterCollision()
{
	m_GotHit = true;
}
void TouhouPlayer_HP::Update()
{
	//här kör vi koden som styr vad som händer när spelaren blir träffad
	if (m_Invincible_Timer == 0 && m_GotHit == 1)
	{
		m_TotalHP -= 1;
		TouhouEngine::PlaySound("Resources/Sounds/Oof.wav", 0.5, "SoundEffect");
		m_Invincible_Timer = 60;
	}
	m_GotHit = 0;
	m_Invincible_Timer -= 1;
	if (m_Invincible_Timer < 0)
	{
		m_Invincible_Timer = 0;
	}
	if (m_Invincible_Timer > 0)
	{
		GetGameObject()->GetComponent<SpriteRenderer>()->ColorKoef.A = std::abs(std::sin(MBMath::DegreeToRadian(m_Invincible_Timer * 6))) * 0.5 + 0.3;
	}
	else
	{
		GetGameObject()->GetComponent<SpriteRenderer>()->ColorKoef.A = 1;
	}
	for (int i = 0; i < m_TotalHP; i++)
	{
		std::array<int, 4> Layer = { 100,1,0,0 };
		MBGE::Transform TextureTransform;
		TextureTransform.SetPosition(Vector2D(5 + i * 1.2f, -3));
		TouhouEngine::DrawTexture("Jakob.png", TextureTransform, 1, 1, Layer);
	}

	if (m_TotalHP <= 0)
	{
		//TouhouEngine::PlaySound("Resources/Sounds/OMyGodNejJohan.wav", 0.5);
		TouhouEngine::Destroy(GetGameObject());
		if (TouhouEngine::FindObjectWithName("Johan") != nullptr)
		{
			TouhouEngine::PlaySound("Resources/Sounds/OMyGodNejJohan.wav", 0.5);
		}
	}
}
//END TouhouPlayer_HP

//BEGIN MBTouhou_Bullet_DeleteOffScreen
void MBTouhou_Bullet_DeleteOffScreen::Update()
{
	//Kanske bör ha en oncreate funktio för sånt här?
	if (m_ObjectRenderer == nullptr)
	{
		m_ObjectRenderer = GetGameObject()->GetComponent<SpriteRenderer>();
	}
	//SpriteRenderer* Renderer = GetComponent<SpriteRenderer>();
	float SpriteWidth = m_ObjectRenderer->Width * GetGameObject()->Transform.GetScaling()[0];
	float SpriteHeight = m_ObjectRenderer->Width * (m_ObjectRenderer->SpriteTexture->GetHeight() / float(m_ObjectRenderer->SpriteTexture->GetWidth())) * GetGameObject()->Transform.GetScaling()[1];
	MBMath::MBVector3<float> ObjectPosition = GetGameObject()->Transform.GetPosition();
	if (ObjectPosition[1] + (SpriteHeight - m_Margin) / 2 > m_ScreenHeight / 2 || ObjectPosition[1] - (SpriteHeight - m_Margin) / 2 < -m_ScreenHeight / 2)
	{
		TouhouEngine::Destroy(GetGameObject());
	}
	if (ObjectPosition[0] + (SpriteWidth - m_Margin) / 2 > m_ScreenWidth / 2 || ObjectPosition[0] - (SpriteWidth - m_Margin) / 2 < -m_ScreenWidth / 2)
	{
		TouhouEngine::Destroy(GetGameObject());
	}
}
//END MBTouhou_Bullet_DeleteOffScreen

//BEGIN MBTouhouRegularEnemy_DeleteOffScreen
void MBTouhouRegularEnemy_DeleteOffScreen::Update()
{
	//Rätt mycket duplicate kod, den enda skillnaden är egentligen bara att fiender deletas när dem är på botten av skärmen
	//Kanske bör ha en oncreate funktio för sånt här?
	if (m_ObjectRenderer == nullptr)
	{
		m_ObjectRenderer = GetGameObject()->GetComponent<SpriteRenderer>();
	}
	//SpriteRenderer* Renderer = GetComponent<SpriteRenderer>();
	float SpriteWidth = m_ObjectRenderer->Width * GetGameObject()->Transform.GetScaling()[0];
	float SpriteHeight = m_ObjectRenderer->Width * (m_ObjectRenderer->SpriteTexture->GetHeight() / float(m_ObjectRenderer->SpriteTexture->GetWidth())) * GetGameObject()->Transform.GetScaling()[1];
	MBMath::MBVector3<float> ObjectPosition = GetGameObject()->Transform.GetPosition();
	if (ObjectPosition[1] - (SpriteHeight - m_Margin) / 2 < -m_ScreenHeight / 2)
	{
		TouhouEngine::Destroy(GetGameObject());
	}
}

//END MBTouhouRegularEnemy_DeleteOffScreen


void MBTouhouEnemy_HP::Update()
{
	if (HP <= 0)
	{
		TouhouEngine::Destroy(GetGameObject());
	}
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

	float HealtbarPercentage = HP / double(MaxHP);
	MBGE::Transform TextureTransform;
	TextureTransform.SetPosition(Vector2D(GetGameObject()->Transform.GetPosition()[0] - (LifeWidth / 2 - (LifeWidth / 2) * HealtbarPercentage), GetGameObject()->Transform.GetPosition()[1] + 0.5f + HealthBarOffset));
	TouhouEngine::DrawTexture("Green.png", TextureTransform, LifeWidth * HealtbarPercentage, LifeHeight, LifeLayer);
	TextureTransform.SetPosition(Vector2D(GetGameObject()->Transform.GetPosition()[0] + (LifeWidth / 2 - (LifeWidth / 2) * (1 - HealtbarPercentage)), GetGameObject()->Transform.GetPosition()[1] + 0.5f + HealthBarOffset));
	TouhouEngine::DrawTexture("RedSquare.png", TextureTransform, LifeWidth * (1 - HealtbarPercentage), LifeHeight, LifeLayer);
}
MBTouhouRegularEnemy::MBTouhouRegularEnemy()
{
	AddComponent(new SpriteRenderer());
	AddComponent(new Rectangle_Hitbox());
	AddComponent(new MBTouhouEnemy_HP());
	AddComponent(new MBTouhouRegularEnemy_DeleteOffScreen());
	//AddComponent()
}