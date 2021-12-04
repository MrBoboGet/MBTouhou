#include <Engine/SpriteAnimationRenderer.h>
#include <MBUtility/MBStrings.h>
#include <fstream>
#include <iostream>
#include <Engine.h>
SpriteAnimationRenderer::SpriteAnimationRenderer(GameObject* ObjectAttachedTo,std::string FilePath)
{
	//ComponentName = "SpriteAnimationRenderer";
	SpelObjectet = ObjectAttachedTo;
	//nu kommer det faktiska l�sandet av filen
    std::ifstream DataSource("Resources/SpelResurser/Sprites/AnimationConfigs/"+FilePath);
    if (DataSource.is_open()) {
        std::string line;
        while (std::getline(DataSource, line))
        {
            auto SplittadString = MBUtility::Split(line, " ");
            //f�rsta v�rdet innan mellanslag �r texturenamnet, andra �r antalet frames den ska spela
            TexturesInAnimation.push_back(SplittadString[0]);
            DurationOfTextures.push_back(std::stoi(SplittadString[1]));
        }
        DataSource.close();
        //all data har laddats in och vi vet att den fungerade
        AnimationData Nydata;
        Nydata.AnimationName = TexturesInAnimation;
        Nydata.AnimationDuration = DurationOfTextures;
        ObjectAnimationsLoaded[FilePath] = Nydata;
    }
    else
    {
        std::cout << "Failed to load animation config!" << std::endl;
    }
    for (size_t i = 0; i < DurationOfTextures.size(); i++)
    {
        NumberOfFrames += DurationOfTextures[i];
        //vi passar p� att ladda in alla textures
        //Texture::LoadTextureFrom("Resources/SpelResurser/Sprites/",TexturesInAnimation[i]);
        TouhouEngine::LoadNamedTexture(TexturesInAnimation[i], "Resources/SpelResurser/Sprites/" + TexturesInAnimation[i]);
    }
}

SpriteAnimationRenderer::~SpriteAnimationRenderer()
{

}

void SpriteAnimationRenderer::Update()
{
	//den nu varande koden totalt f�ruts�tter att alla sprites �r lika stora, ingen h�nsyn tas till hur stora dem �r relativt
    //TODO Implementera en b�ttre pipeline s� att ist�llet f�r att �ndra texturen spel objectet har �r allt som dras till sk�rmen en draw call, �ven render funktionen av spel object, s� kan vi g�r detta lite b�ttre

    //�ndrar texturen beroende p� configsen vi laddat in
    int AntalFramesInIAnimationen = 0;
    for (int i = 0; i < TexturesInAnimation.size(); i++)
    {
        AntalFramesInIAnimationen += DurationOfTextures[i];
        if (AnimationTimer < AntalFramesInIAnimationen)
        {
            //SpelObjectet->Renderer.ObjectTexture =/* Egentligen �r detta fett farligt, har ingen anninge vad detta g�r egentligen TODO t�nk igenom om dett fugnerar*/ *(Texture::LoadTextureFrom("SpelResurser/Sprites/", TexturesInAnimation[i]));
            SpelObjectet->GetComponent<SpriteRenderer>()->SpriteTexture = TouhouEngine::LoadNamedTexture(TexturesInAnimation[i], "SpelResurser/Sprites/" + TexturesInAnimation[i]);
            break;
        }
    }
    AnimationTimer += 1;
    if (AnimationTimer >= NumberOfFrames)
    {
        AnimationTimer = 0;
    }
}
void SpriteAnimationRenderer::PlayAnimation(std::string ConfigName)
{
    if (ObjectAnimationsLoaded.find(ConfigName) != ObjectAnimationsLoaded.end())
    {
        AnimationData Datan = ObjectAnimationsLoaded[ConfigName];
        TexturesInAnimation = Datan.AnimationName;
        DurationOfTextures = Datan.AnimationDuration;
        AnimationTimer = 0;
    }
    else
    {
        AnimationData NyData;
        NyData.AnimationDuration = std::vector<int>(0);
        NyData.AnimationName = std::vector<std::string>(0);
        std::ifstream DataSource("Resources/SpelResurser/Sprites/AnimationConfigs/" + ConfigName);
        if (DataSource.is_open()) {
            std::string line;
            while (std::getline(DataSource, line))
            {
                auto SplittadString = MBUtility::Split(line, " ");
                //f�rsta v�rdet innan mellanslag �r texturenamnet, andra �r antalet frames den ska spela
                NyData.AnimationName.push_back(SplittadString[0]);
                NyData.AnimationDuration.push_back(std::stoi(SplittadString[1]));
            }
            DataSource.close();
        }
        ObjectAnimationsLoaded[ConfigName] = NyData;
        TexturesInAnimation = NyData.AnimationName;
        DurationOfTextures = NyData.AnimationDuration;
        AnimationTimer = 0;
    }
}