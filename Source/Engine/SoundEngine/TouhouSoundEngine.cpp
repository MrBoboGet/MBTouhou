#include <TouhouSoundEngine.h>
#include <fmod.hpp>
#include <assert.h>
#include <Engine.h>
#include <iostream>
TouhouSoundEngine* TouhouSoundEngine::ActiveEngine = nullptr;


TouhouSoundEngine::TouhouSoundEngine()
{
    //s�tter nullptr till denna engine
    if (ActiveEngine == nullptr)
    {
        ActiveEngine = this;
    }
    else
    {
        assert(false);
    }
}

TouhouSoundEngine::~TouhouSoundEngine()
{

}
void TouhouSoundEngine::Initialize()
{
    FMOD_RESULT result;
    result = FMOD::System_Create(&FmodSystem);      // Create the main system object.
    if (result != FMOD_OK)
    {
        //error handling
        assert(false);
    }
    result = FmodSystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
    if (result != FMOD_OK)
    {
        //error handling
        assert(false);
    }
}
void TouhouSoundEngine::PlaySound(std::string Path, float Volume)
{
    //vi laddar in ljudet om det inte redan finns, anv�nder f�r cache annars
    if (CachedSounds.find(Path) != CachedSounds.end())
    {
        //spela ljudet
        FMOD::Channel* ChannelAtt�ndraVolymenP�;
        FmodSystem->playSound(CachedSounds[Path], nullptr, true, &ChannelAtt�ndraVolymenP�);
        float VolumeOfChannel;
        ChannelAtt�ndraVolymenP�->getVolume(&VolumeOfChannel);
        ChannelAtt�ndraVolymenP�->setVolume(VolumeOfChannel * Volume);
        ChannelAtt�ndraVolymenP�->setPaused(false);
    }
    else
    {
        //skapar ljudet
        FMOD::Sound* NewSound;
        FMOD_RESULT SkapaLjudet = FmodSystem->createSound(Path.c_str(),FMOD_CREATESAMPLE,nullptr, &NewSound);
        if (SkapaLjudet != FMOD_OK)
        {
            std::cout << "Fel Med skapandet av ljudet " << SkapaLjudet << std::endl;
            return;
        }
        CachedSounds[Path] = NewSound;
        //nu spelar vi ljuder efter vi skapat det
        FMOD::Channel* ChannelAtt�ndraVolymenP�;
        FMOD_RESULT SpelaLjudet = FmodSystem->playSound(CachedSounds[Path], nullptr, true, &ChannelAtt�ndraVolymenP�);
        float VolumeOfChannel;
        ChannelAtt�ndraVolymenP�->getVolume(&VolumeOfChannel);
        ChannelAtt�ndraVolymenP�->setVolume(VolumeOfChannel * Volume);
        ChannelAtt�ndraVolymenP�->setPaused(false);
        if (SpelaLjudet != FMOD_OK)
        {
            std::cout <<"Fel Med Spelandet av ljudet "<<SpelaLjudet << std::endl;
        }
    }
}
void TouhouSoundEngine::PlaySound(TouhouSoundEngineSoundObject* SoundToPlay)
{
    if (SoundToPlay->IsPlaying())
    {
        return;
    }
    if (CachedSounds.find(SoundToPlay->Sound) != CachedSounds.end())
    {
        FmodSystem->playSound(CachedSounds[SoundToPlay->Sound], nullptr, false, &SoundToPlay->ChannelPlayingOn.EngineChannelPointer);
    }
    else
    {
        FMOD::Sound* NewSound;
        FMOD_RESULT SkapaLjudet = FmodSystem->createSound(SoundToPlay->Sound.c_str(), FMOD_CREATESAMPLE, nullptr, &NewSound);
        if (SkapaLjudet != FMOD_OK)
        {
            std::cout << "Fel Med skapandet av ljudet " << SkapaLjudet << std::endl;
            return;
        }
        CachedSounds[SoundToPlay->Sound] = NewSound;
        //nu spelar vi ljuder efter vi skapat det
        FMOD_RESULT SpelaLjudet = FmodSystem->playSound(CachedSounds[SoundToPlay->Sound], nullptr, false, nullptr);
        if (SpelaLjudet != FMOD_OK)
        {
            std::cout << "Fel Med Spelandet av ljudet " << SpelaLjudet << std::endl;
        }
    }
}
void TouhouSoundEngine::PlaySound(TouhouSoundEngineSoundObject* SoundToPlay, float PercentOfMax)
{
    if (SoundToPlay->IsPlaying())
    {
        return;
    }
    if (CachedSounds.find(SoundToPlay->Sound) != CachedSounds.end())
    {
        FmodSystem->playSound(CachedSounds[SoundToPlay->Sound], nullptr, true, &SoundToPlay->ChannelPlayingOn.EngineChannelPointer);
        FMOD::Channel* ChannelAtt�ndraVolymenP� = SoundToPlay->ChannelPlayingOn.EngineChannelPointer;
        float VolumeOfChannel;
        ChannelAtt�ndraVolymenP�->getVolume(&VolumeOfChannel);
        ChannelAtt�ndraVolymenP�->setVolume(PercentOfMax*VolumeOfChannel);
        ChannelAtt�ndraVolymenP�->setPaused(false);
    }
    else
    {
        FMOD::Sound* NewSound;
        FMOD_RESULT SkapaLjudet = FmodSystem->createSound(SoundToPlay->Sound.c_str(), FMOD_CREATESAMPLE, nullptr, &NewSound);
        if (SkapaLjudet != FMOD_OK)
        {
            std::cout << "Fel Med skapandet av ljudet " << SkapaLjudet << std::endl;
            return;
        }
        CachedSounds[SoundToPlay->Sound] = NewSound;
        //nu spelar vi ljuder efter vi skapat det
        FMOD_RESULT Error;
        Error = FmodSystem->playSound(CachedSounds[SoundToPlay->Sound], nullptr, true, &SoundToPlay->ChannelPlayingOn.EngineChannelPointer);
        FMOD::Channel* ChannelAtt�ndraVolymenP� = SoundToPlay->ChannelPlayingOn.EngineChannelPointer;
        float VolumeOfChannel;
        Error = ChannelAtt�ndraVolymenP�->getVolume(&VolumeOfChannel);
        Error = ChannelAtt�ndraVolymenP�->setVolume(PercentOfMax*VolumeOfChannel);
        Error = ChannelAtt�ndraVolymenP�->getVolume(&VolumeOfChannel);
        Error = ChannelAtt�ndraVolymenP�->setPaused(false);
    }
}
bool TouhouSoundEngine::IsSoundPlaying(TouhouSoundEngineSoundObject* Sound)
{
    bool IsPlaying;
    Sound->ChannelPlayingOn.EngineChannelPointer->isPlaying(&IsPlaying);
    return(IsPlaying);
}
void TouhouSoundEngine::SetPause(TouhouSoundEngineSoundObject* SoundToPlay,bool Value)
{
    if (SoundToPlay->ChannelPlayingOn.EngineChannelPointer != nullptr)
    {
        SoundToPlay->ChannelPlayingOn.EngineChannelPointer->setPaused(Value);
    }
}
void TouhouSoundEngine::Update()
{
    FmodSystem->update();
}
void TouhouSoundEngine::LoadSound(std::string Path)
{

}

//sound object

TouhouSoundEngineSoundObject::TouhouSoundEngineSoundObject(std::string SoundToPlay, std::string Tag)
{
    this->Tag = Tag;
    Sound = SoundToPlay;
    //l�gger till den i den stora listan av soundobjects
    TouhouEngine::SoundObjects.push_back(this);
    ActiveSoundEngine = GetActiveEngine();
}
TouhouSoundEngine* TouhouSoundEngineSoundObject::GetActiveEngine()
{
    return(TouhouSoundEngine::ActiveEngine);
}
bool TouhouSoundEngineSoundObject::IsPlaying()
{
    return(ActiveSoundEngine->IsSoundPlaying(this));
}
void TouhouSoundEngineSoundObject::PlaySound()
{
    ActiveSoundEngine->PlaySound(this);
}
void TouhouSoundEngineSoundObject::PlaySound(float PercentMaxVolume)
{
    ActiveSoundEngine->PlaySound(this,PercentMaxVolume);
}
void TouhouSoundEngineSoundObject::SetPause(bool Value)
{
    ActiveSoundEngine->SetPause(this,Value);
}
TouhouSoundEngineSoundObject::~TouhouSoundEngineSoundObject()
{
    //tar bort detta objekt ur listan
    int PositionIListan = -1;
    for (int i = 0; i < TouhouEngine::SoundObjects.size(); i++)
    {
        if (TouhouEngine::SoundObjects[i] == this)
        {
            PositionIListan = i;
            break;
        }
    }
    TouhouEngine::SoundObjects.erase(TouhouEngine::SoundObjects.begin() + PositionIListan);
}


//channel object

TouhouSoundEngineChannel::TouhouSoundEngineChannel()
{

}

TouhouSoundEngineChannel::~TouhouSoundEngineChannel()
{

}