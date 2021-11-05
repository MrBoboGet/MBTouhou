#pragma once
#include <Engine.h>
class PausMenu : public GameObject
{
private:
	bool Paused = false;
	int CurrentOption = 0;
	int NumberOfOptions = 3;
public:
	virtual void Update() override;

	PausMenu();
	~PausMenu();

};