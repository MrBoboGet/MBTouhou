#include <Player_Attack_BigShot.h>
#include <Engine.h>
#include <Engine\SpriteAnimationRenderer.h>
#include <assert.h>
Player_Attack_BigShot::Player_Attack_BigShot(Player* Spelaren)
{
	this->Spelaren = Spelaren;
}	

Player_Attack_BigShot::~Player_Attack_BigShot()
{
}
void Player_Attack_BigShot::Update()
{
	Timer -= 1;
	if (Spelaren->Key_C)
	{
		//vi minska
		if (Spelaren->CurrentEnergy >= EnergyCost)
		{
			if (Timer < 0)
			{
				//vi har en liten delay mellan varje, men nu skjuter vi det stora skottet
				Spelaren->CurrentEnergy -= EnergyCost;
				Timer = ShotDelay;

				//nu skapar vi ju faktiskt big bad skottet, vilket egentligen är det viktiga
				Player_Bullet* NyBullet = static_cast<Player_Bullet*>(TouhouEngine::Create(new Player_Bullet(Spelaren->Position, "BigShot.png", 0.32f)));
				NyBullet->Speed = 0.24f;
				NyBullet->Direction = 90;
				NyBullet->Damage = 10;
				NyBullet->Hitbox = Vector2D(0.4f, 0.4f);
				TouhouEngine::PlaySound("Resources/Sounds/BigShot.wav",0.2,"SoundEffect");

				//som litet test ska vi nu testa att byta animationen också
				//SpriteAnimationRenderer* SpriteRendern = static_cast<SpriteAnimationRenderer*>((Spelaren->GetComponent("SpriteAnimationRenderer")));
				//assert(SpriteRendern != nullptr);
				//SpriteRendern->PlayAnimation("PlayerAnimationConfig2.txt");
			}
		}
	}
}