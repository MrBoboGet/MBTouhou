#include <Engine.h>

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
class MBTouhouRegularEnemy : public GameObject
{
private:

public:
	MBTouhouRegularEnemy();
};