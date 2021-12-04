#include <Engine/SpriteAnimationRenderer.h>
#include <MBUtility/MBStrings.h>
#include <fstream>
#include <iostream>
#include <Engine.h>
SpriteAnimationRenderer::SpriteAnimationRenderer(GameObject* ObjectAttachedTo,std::string FilePath)
{
	//ComponentName = "SpriteAnimationRenderer";
	SpelObjectet = ObjectAttachedTo;
	//nu kommer det faktiska läsandet av filen
    std::ifstream DataSource("Resources/SpelResurser/Sprites/AnimationConfigs/"+FilePath);
    if (DataSource.is_open()) {
        std::string line;
        while (std::getline(DataSource, line))
        {
            auto SplittadString = MBUtility::Split(line, " ");
            //första värdet innan mellanslag är texturenamnet, andra är antalet frames den ska spela
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
        //vi passar på att ladda in alla textures
        //Texture::LoadTextureFrom("Resources/SpelResurser/Sprites/",TexturesInAnimation[i]);
        TouhouEngine::LoadNamedTexture(TexturesInAnimation[i], "Resources/SpelResurser/Sprites/" + TexturesInAnimation[i]);
    }
}

SpriteAnimationRenderer::~SpriteAnimationRenderer()
{

}

void SpriteAnimationRenderer::Update()
{
	//den nu varande koden totalt förutsätter att alla sprites är lika stora, ingen hänsyn tas till hur stora dem är relativt
    //TODO Implementera en bättre pipeline så att istället för att ändra texturen spel objectet har är allt som dras till skärmen en draw call, även render funktionen av spel object, så kan vi gör detta lite bättre

    //ändrar texturen beroende på configsen vi laddat in
    int AntalFramesInIAnimationen = 0;
    for (int i = 0; i < TexturesInAnimation.size(); i++)
    {
        AntalFramesInIAnimationen += DurationOfTextures[i];
        if (AnimationTimer < AntalFramesInIAnimationen)
        {
            //SpelObjectet->Renderer.ObjectTexture =/* Egentligen är detta fett farligt, har ingen anninge vad detta gör egentligen TODO tänk igenom om dett fugnerar*/ *(Texture::LoadTextureFrom("SpelResurser/Sprites/", TexturesInAnimation[i]));
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
                //första värdet innan mellanslag är texturenamnet, andra är antalet frames den ska spela
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