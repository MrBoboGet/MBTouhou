#pragma once
#include <Engine\Engine_GenericComponent.h>
#include <vector>
#include <Gameobject.h>
//struct AnimationData
//{
//	std::vector<std::string> AnimationName;
//	std::vector<int> AnimationDuration;
//};
//class SpriteAnimationRenderer : public Component
//{
//public:
//	int AnimationTimer = 0;
//	int NumberOfFrames = 0;
//	std::vector<std::string> TexturesInAnimation = std::vector<std::string>(0);
//	std::vector<int> DurationOfTextures = std::vector<int>(0);
//	std::unordered_map<std::string, AnimationData> ObjectAnimationsLoaded = std::unordered_map<std::string, AnimationData>(0);
//	SpriteAnimationRenderer(GameObject* ObjectAttachedTo,std::string AnimationConfigFilePath);
//	void PlayAnimation(std::string ConfigName);
//	virtual void Update() override;
//	~SpriteAnimationRenderer();
//
//private:
//
//};