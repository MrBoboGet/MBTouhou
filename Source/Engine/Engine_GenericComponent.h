#pragma once
#include <string>
#include <vector>
class GameObject;
class Component
{
protected:
	GameObject* SpelObjectet;
public:
	std::string ComponentName;
	virtual std::vector<std::string> RequiredResources();
	Component();
	~Component();
	virtual void Update();
private:

};
