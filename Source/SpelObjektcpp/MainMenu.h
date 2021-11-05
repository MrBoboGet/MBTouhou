#pragma once
#include <Engine.h>
class MainMenu : public GameObject
{
private:
	int CurrentOption = 0;
	int NumberOfOptions = 3;
	bool InSubMenu = false;
public:
	MainMenu();
	virtual void Update();
	~MainMenu();
};