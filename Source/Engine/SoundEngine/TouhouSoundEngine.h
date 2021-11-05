#pragma once
#include <fmod.hpp>
#include <string>
#include <unordered_map>
class TouhouSoundEngine;
class TouhouSoundEngineChannel
{
	friend class TouhouSoundEngine;
private:
	FMOD::Channel* EngineChannelPointer = nullptr;
public:
	TouhouSoundEngineChannel();
	~TouhouSoundEngineChannel();
};
class TouhouSoundEngineSoundObject
{
	friend class TouhouSoundEngine;
private:
	std::string Tag;
	std::string Sound;
	TouhouSoundEngineChannel ChannelPlayingOn;
	TouhouSoundEngine* ActiveSoundEngine;
	bool Playing = false;
	float DefaultRelativeVolume = 1;
public:
	bool DynamicallyAllocatedAndTemporary = false;
	void PlaySound();
	void PlaySound(float PercentOfMaxVolume);
	bool IsPlaying();
	void SetPause(bool Valuer);
	TouhouSoundEngine* GetActiveEngine();
	//void SetVolume(float Volume);
	TouhouSoundEngineSoundObject(std::string SoundToPlay, std::string Tag = "");
	~TouhouSoundEngineSoundObject();

};

//själva enginen för att spela ljud, ska inte ha några referenser till vår engine
class TouhouSoundEngine
{
private:
	FMOD::System* FmodSystem = nullptr;
	std::unordered_map<std::string, FMOD::Sound*> CachedSounds = std::unordered_map<std::string, FMOD::Sound*>(0);
	static TouhouSoundEngine* ActiveEngine;
public:
	friend TouhouSoundEngine* TouhouSoundEngineSoundObject::GetActiveEngine();
	TouhouSoundEngine();
	~TouhouSoundEngine();
	void PlaySound(std::string Path,float Volume);
	void PlaySound(TouhouSoundEngineSoundObject* SoundObjectToStartPlaying);
	void PlaySound(TouhouSoundEngineSoundObject* SoundObjectToStartPlaying,float PercentOfMaxVolume);
	void SetPause(TouhouSoundEngineSoundObject* SoundObjectToStopPlaying, bool Value);
	bool IsSoundPlaying(TouhouSoundEngineSoundObject* Sound);
	void LoadSound(std::string Path);
	void Initialize();
	void Update();
};