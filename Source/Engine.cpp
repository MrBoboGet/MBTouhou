#include <Engine.h>
#include <ctime>
#include <iostream>
std::unordered_map<std::string, Shader*> TouhouEngine::LoadedShaders = {};
std::unordered_map<std::string, Texture*> TouhouEngine::LoadedTextures = {};
GLFWwindow* TouhouEngine::CurrentWindow = nullptr;

std::vector<GameObject*> TouhouEngine::ActiveGameObjects = {};
std::vector<GameObject*> TouhouEngine::DeletedGameObjects = {};
std::vector<DrawObject*> TouhouEngine::DrawCalls = {};
float TouhouEngine::Framerate = 0.0166666f;
bool TouhouEngine::UpdatePause = false;

std::vector<std::string> TouhouEngine::KeyList = std::vector<std::string>(0);
std::unordered_map<std::string, bool> TouhouEngine::PressedKeys = std::unordered_map<std::string, bool>(0);
std::unordered_map<std::string, bool> TouhouEngine::PreviouslyPressedKeys = std::unordered_map<std::string, bool>(0);

TouhouSoundEngine TouhouEngine::SoundEngine = TouhouSoundEngine();
std::vector<TouhouSoundEngineSoundObject*> TouhouEngine::SoundObjects = std::vector<TouhouSoundEngineSoundObject*>(0);
//std::vector<void(*)()> TouhouEngine::CustomUpdateProcedures = std::vector<void(*)()>(0);

TouhouEngine::TouhouEngine()
{
}

TouhouEngine::~TouhouEngine()
{
}
GameObject* TouhouEngine::Create(GameObject* a)
{
	ActiveGameObjects.push_back(a);
	return(a);
}
void TouhouEngine::Destroy(GameObject* a)
{
	int PositionIListan = 0;
	for (int i = 0; i < ActiveGameObjects.size();i++)
	{
		if (ActiveGameObjects[i] == a)
		{
			PositionIListan = i;
			break;
		}
	}
	//OBS!!! Vi m�te erasa efter vi deletat grejer, annars s� kommer vi f� fel
	ActiveGameObjects[PositionIListan]->Active = false;
	DeletedGameObjects.push_back(ActiveGameObjects[PositionIListan]);
	ActiveGameObjects.erase(ActiveGameObjects.begin() + PositionIListan);
}
GameObject* TouhouEngine::FindObjectWithName(std::string Namn)
{
	//just nu har vi inte ett b�ttre system �n att vi g�r igenom objekten i tur och ordning till vi hittar ett f�rem�l med det namnet. Vi kanske vill ha en hashlista i framtiden
	for (int i = 0; i < ActiveGameObjects.size(); i++)
	{
		if (ActiveGameObjects[i]->GetName() == Namn)
		{
			return(ActiveGameObjects[i]);
		}
	}
	//nu har allts� inget namn hittats, detta borde egentligen krascha programmet
	return(nullptr);
}
GameObject* TouhouEngine::FindObjectWithTag(std::string const& Tag)
{
	//just nu har vi inte ett b�ttre system �n att vi g�r igenom objekten i tur och ordning till vi hittar ett f�rem�l med det namnet. Vi kanske vill ha en hashlista i framtiden
	for (int i = 0; i < ActiveGameObjects.size(); i++)
	{
		if (ActiveGameObjects[i]->GetTag() == Tag)
		{
			return(ActiveGameObjects[i]);
		}
	}
	//nu har allts� inget namn hittats, detta borde egentligen krascha programmet
	return(nullptr);
}
Shader* TouhouEngine::LoadShader(std::string ShaderName, std::string VertexFilepath, std::string FragmentFilepath)
{
	Shader* NyaShadern = new Shader("Resources/Shaders/"+VertexFilepath, "Resources/Shaders/"+FragmentFilepath);
	LoadedShaders[ShaderName] = NyaShadern;
	return(NyaShadern);
}
void TouhouEngine::UpdateGameobjects(std::vector<GameObject*> GameobjectsToUpdate)
{
	for (int i = 0; i < GameobjectsToUpdate.size();i++)
	{
		if (GameobjectsToUpdate[i]->Active)
		{
			GameobjectsToUpdate[i]->Update();
		}
	}
}
bool LayerIsBigger(std::vector<int> Layer1, std::vector<int> Layer2)
{
	bool IsBigger = false;
	for (int i = 0; i < Layer1.size();i++)
	{
		if (i < Layer2.size())
		{
			if (Layer1[i] > Layer2[i])
			{
				IsBigger = true;
				break;
			}
		}
		else
		{
			break;
		}
	}
	return(IsBigger);
}
void TouhouEngine::Render(std::vector<GameObject*> Lista)
{
	if (Lista.size() > 1)
	{
		for (int i = 0;i < Lista.size() - 1; i++)
		{
			for (int j = 0; j < Lista.size() - 1 - i; j++)
			{
				//sj�lva j�mf�relsen, bygger p� hur Layer systemet fungerar
				if (LayerIsBigger(Lista[j]->Renderer.Layer, Lista[j + 1]->Renderer.Layer) == true)
				{
					//D� byter vi plats p� dem
					auto Temp = Lista[j];
					Lista[j] = Lista[j + 1];
					Lista[j + 1] = Temp;
				}
			}
		}
	}
	if (TouhouEngine::DrawCalls.size() > 1)
	{
		for (int i = 0;i < TouhouEngine::DrawCalls.size() - 1; i++)
		{
			for (int j = 0; j < TouhouEngine::DrawCalls.size() - 1 - i; j++)
			{
				//sj�lva j�mf�relsen, bygger p� hur Layer systemet fungerar
				if (LayerIsBigger(TouhouEngine::DrawCalls[j]->Layer, TouhouEngine::DrawCalls[j + 1]->Layer) == true)
				{
					//D� byter vi plats p� dem
					auto Temp = TouhouEngine::DrawCalls[j];
					TouhouEngine::DrawCalls[j] = TouhouEngine::DrawCalls[j + 1];
					TouhouEngine::DrawCalls[j + 1] = Temp;
				}
			}
		}
	}
	int GameObjectIterator = 0;
	int DrawObjectIterator = 0;
	while (GameObjectIterator != Lista.size())
	{
		if (DrawObjectIterator != TouhouEngine::DrawCalls.size())
		{
			if (LayerIsBigger(Lista[GameObjectIterator]->Renderer.Layer, TouhouEngine::DrawCalls[DrawObjectIterator]->Layer) == false)
			{
				Lista[GameObjectIterator]->Render();
				GameObjectIterator += 1;
			}
			else
			{
				while (LayerIsBigger(Lista[GameObjectIterator]->Renderer.Layer, TouhouEngine::DrawCalls[DrawObjectIterator]->Layer) == true)
				{
					Texture::DrawCall(TouhouEngine::DrawCalls[DrawObjectIterator]);
					DrawObjectIterator += 1;
				}
			}
		}
		else
		{
			Lista[GameObjectIterator]->Render();
			GameObjectIterator += 1;
		}
		if (GameObjectIterator == Lista.size())
		{
			if (DrawObjectIterator != TouhouEngine::DrawCalls.size())
			{
				while (DrawObjectIterator != TouhouEngine::DrawCalls.size())
				{
					Texture::DrawCall(TouhouEngine::DrawCalls[DrawObjectIterator]);
					DrawObjectIterator += 1;
				}
			}
		}
	}
	//efter den h�t sinnessjukt l�nga koden f�r n�got mycket enkelt, f�r kolla �ver om jag kan g�ra den b�ttre, s� vill vi deleta alla draw calls
	for (int i = 0; i < TouhouEngine::DrawCalls.size();i++)
	{
		delete TouhouEngine::DrawCalls[i];
	}
	TouhouEngine::DrawCalls = {};
	/*
	for (int i = 0; i < Lista.size();i++)
	{
		Lista[i]->Render();
	}
	*/
}
void TouhouEngine::InitializeEngineCompononets()
{
	SoundEngine.Initialize();
}
void TouhouEngine::UpdateSoundSystem()
{
	SoundEngine.Update();
}
void TouhouEngine::CleanupFinishedSounds()
{
	//int SizeOfSoundObjects = SoundObjects.size();
	//std::vector<int> IndexOfSoundsToRemove = std::vector<int>(0);
	//for (size_t i = 0; i < SizeOfSoundObjects; i++)
	//{
	//	if (SoundObjects[i]->DynamicallyAllocatedAndTemporary)
	//	{
	//		if (!SoundObjects[i]->IsPlaying())
	//		{
	//			IndexOfSoundsToRemove.push_back(i);
	//		}
	//	}
	//}
	//int SizeOfSoundToRemove = IndexOfSoundsToRemove.size();
	//for (size_t i = 0; i < SizeOfSoundToRemove; i++)
	//{
	//	delete SoundObjects[i];
	//	SoundObjects.erase(SoundObjects.begin() + IndexOfSoundsToRemove[i] - i);
	//}
}
void TouhouEngine::Mainloop()
{
	clock_t Timer = clock();
	int FrameCount = 0;
	InitializeEngineCompononets();
	while (!glfwWindowShouldClose(TouhouEngine::CurrentWindow))
	{
		if (float(clock() - Timer) / CLOCKS_PER_SEC > Framerate)
		{
			if (float(clock() - Timer) / CLOCKS_PER_SEC > Framerate + 0.01)
			{
				std::cout << "Fps is not exactly 60" << std::endl;
			}
			glClear(GL_COLOR_BUFFER_BIT);
			FrameCount += 1;
			Timer = clock();
			UpdateSoundSystem();
			HandleDeletedGameobjects();
			std::vector<GameObject*> CurrentlyActiveGameObjects = ActiveGameObjects;
			clock_t Timer2 = clock();
			UpdateKeys();
			
			if (!UpdatePause)
			{
				UpdateGameobjects(CurrentlyActiveGameObjects);
			}
			if (FrameCount % 60 == 0)
			{
				std::cout << "Update Time is " << float(clock() - Timer2) / CLOCKS_PER_SEC << std::endl;
			}
			Timer2 = clock();
			Render(CurrentlyActiveGameObjects);
			if (FrameCount % 60 == 0)
			{
				std::cout << "Render time is " << float(clock() - Timer2) / CLOCKS_PER_SEC << std::endl;
			}
			DeleteObjectsOutsideScreen();
			glfwSwapBuffers(TouhouEngine::CurrentWindow);
		}
		glfwPollEvents();
	}
}
void TouhouEngine::DeleteObjectsOutsideScreen()
{
	std::vector<int> ObjectOutside;
	for (int i = 0;i < TouhouEngine::ActiveGameObjects.size();i++)
	{
		if (TouhouEngine::ActiveGameObjects[i]->GetTag() == "Enemy_Bullet")
		{
			//i objektet s� l�ter vi f�rh�llandet vara dynamiskt, men jag orkar inte fixa det s� jag hardcodar det
			//borde ocks� vara enkelt att se om det blir fel, objekt f�rsvinner fr�n sk�rmen innan dem borde
			auto YPositionGlTop = TouhouEngine::ActiveGameObjects[i]->Position.y / 4.5 + TouhouEngine::ActiveGameObjects[i]->Renderer.Size * ((float)16 / (float)9) * ((float)TouhouEngine::ActiveGameObjects[i]->Renderer.ObjectTexture.GetHeight() / (float)TouhouEngine::ActiveGameObjects[i]->Renderer.ObjectTexture.GetWidth()) / 8;
			auto YPositionGlBottom = TouhouEngine::ActiveGameObjects[i]->Position.y / 4.5 - TouhouEngine::ActiveGameObjects[i]->Renderer.Size * ((float)16 / (float)9) * ((float)TouhouEngine::ActiveGameObjects[i]->Renderer.ObjectTexture.GetHeight() / (float)TouhouEngine::ActiveGameObjects[i]->Renderer.ObjectTexture.GetWidth()) / 8;
			//TODO faktiskt ta sig tiden att kolla och se till att denna kod funkar
			auto XPositionLeft = TouhouEngine::ActiveGameObjects[i]->Position.x / 8 + TouhouEngine::ActiveGameObjects[i]->Renderer.Size / 8;
			auto XPositionRight = TouhouEngine::ActiveGameObjects[i]->Position.x / 8 - TouhouEngine::ActiveGameObjects[i]->Renderer.Size / 8;
			if (YPositionGlTop < -1 || YPositionGlBottom>1 || XPositionLeft < -1 || XPositionRight > 1)
			{
				//om den �r utanf�r sk�rmen, spriten inte syns, deletar vi den helt enkelt
				ObjectOutside.push_back(i);
				TouhouEngine::DeletedGameObjects.push_back(TouhouEngine::ActiveGameObjects[i]);
			}
		}
		if (TouhouEngine::ActiveGameObjects[i]->GetTag() == "Enemy")
		{
			if (TouhouEngine::ActiveGameObjects[i]->Position.y / 4.5 + TouhouEngine::ActiveGameObjects[i]->Renderer.Size * ((float)16 / (float)9) * ((float)TouhouEngine::ActiveGameObjects[i]->Renderer.ObjectTexture.GetHeight() / (float)TouhouEngine::ActiveGameObjects[i]->Renderer.ObjectTexture.GetWidth()) / 8 < -1)
			{
				ObjectOutside.push_back(i);
				TouhouEngine::DeletedGameObjects.push_back(TouhouEngine::ActiveGameObjects[i]);
			}
		}
		if (TouhouEngine::ActiveGameObjects[i]->GetTag() == "Player_Bullet")
		{
			auto Objektet = TouhouEngine::ActiveGameObjects[i];
			if ((Objektet->Position.y / 4.5) - Objektet->Renderer.Size * ((float)16 / (float)9) * (Objektet->Renderer.ObjectTexture.GetHeight() / (float)Objektet->Renderer.ObjectTexture.GetWidth()) / 8 > 1)
			{
				//spelar kulorna �r �ver sk�rmen
				ObjectOutside.push_back(i);
				TouhouEngine::DeletedGameObjects.push_back(TouhouEngine::ActiveGameObjects[i]);
			}
		}
	}
	for (int i = 0; i < ObjectOutside.size();i++)
	{
		TouhouEngine::ActiveGameObjects.erase(TouhouEngine::ActiveGameObjects.begin() + (ObjectOutside[i] - i));
	}
}
void TouhouEngine::HandleDeletedGameobjects()
{
	//nu deletar vi alla f�rem�l som vi ville ha bort f�rra framen
	for (int i = 0; i < TouhouEngine::DeletedGameObjects.size();i++)
	{
		delete TouhouEngine::DeletedGameObjects[i];
	}
	TouhouEngine::DeletedGameObjects = {};
}
void TouhouEngine::InitializeWindow(int Width, int Height, std::string WindowName, GLFWmonitor* Monitor, GLFWwindow* Share)
{
	TouhouEngine::CurrentWindow = glfwCreateWindow(Width, Height, WindowName.c_str(), NULL, Share);
	if (!TouhouEngine::CurrentWindow)
	{
		glfwTerminate();
		ASSERT(false);
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(TouhouEngine::CurrentWindow);
	if (glewInit() == GLEW_OK)
	{
		std::cout << "det funkar" << std::endl;
	}
	//ser till att det fins blending, �ndra inte vet inte hur den funkar
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));
	//den h�r koden g�r s� att v-sync �r p�
	glfwSwapInterval(1);
}

//keyPresses

std::unordered_map<std::string, int> GLFWKeycodes = std::unordered_map<std::string, int>(0);
int CharacterToGLFWKey(std::string Character)
{
	if (GLFWKeycodes.find(Character) != GLFWKeycodes.end())
	{
		return(GLFWKeycodes[Character]);
	}
	else
	{
		//innebn�r att det inte �r en prinbar key, vi har ej support f�r det �n s� l�nge
		return(-1);
	}
}
bool InitializedKeys = false;
bool MapGenerated = false;
void TouhouEngine::UpdateKeys()
{
	if (!MapGenerated)
	{
		//generar en map f�r alla printable keys
		for (size_t i = 1; i < 348; i++)
		{
			if (glfwGetKeyName(i, 0) != NULL)
			{
				GLFWKeycodes[glfwGetKeyName(i, 0)] = i;
			}
		}
		//lite hardcodace v�rden
		GLFWKeycodes["space"] = 32;
		GLFWKeycodes["esc"] = 256;
		GLFWKeycodes["tab"] = 258;
		GLFWKeycodes["left"] = 263;
		GLFWKeycodes["right"] = 262;
		GLFWKeycodes["up"] = 265;
		GLFWKeycodes["down"] = 264;
		GLFWKeycodes["leftshift"] = 340;
		GLFWKeycodes["enter"] = 257;
		MapGenerated = true;
	}
	if (!InitializedKeys)
	{
		KeyList.reserve(GLFWKeycodes.size());
		for (auto kv : GLFWKeycodes) 
		{
			KeyList.push_back(kv.first);
		}
		InitializedKeys = true;
		//uppdaterar v�ra values, men s�tter prevuious direkt till falsk
		for (size_t i = 0; i < KeyList.size(); i++)
		{
			PreviouslyPressedKeys[KeyList[i]] = false;
			PressedKeys[KeyList[i]] = false;
		}
	}
	else
	{
		PreviouslyPressedKeys = PressedKeys;
		for (size_t i = 0; i < KeyList.size(); i++)
		{
			PressedKeys[KeyList[i]] = glfwGetKey(CurrentWindow,GLFWKeycodes[KeyList[i]]);
		}
	}
}
bool TouhouEngine::GetKeyDown(std::string Key)
{
	return(PressedKeys[Key]);
}
bool TouhouEngine::GetKeyPressed(std::string Key)
{
	if(PressedKeys[Key] == true && PreviouslyPressedKeys[Key] == false)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}
bool TouhouEngine::GetKeyReleased(std::string Key)
{
	if (PressedKeys[Key] == false && PreviouslyPressedKeys[Key] == true)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}
void TouhouEngine::_GetWindowSize(int* Width, int* Height)
{
	glfwGetWindowSize(TouhouEngine::CurrentWindow, Width, Height);

}
ActiveObjectIterator TouhouEngine::GetActiveObjectsIterator()
{
	ActiveObjectIterator ReturnValue;
	ReturnValue.m_InternalIterator = TouhouEngine::ActiveGameObjects.begin();
	ReturnValue.m_EndIterator = TouhouEngine::ActiveGameObjects.end();
	return(ReturnValue);
}
void TouhouEngine::ClearObjects()
{
	std::vector<GameObject*> ObjectsToDelete = TouhouEngine::ActiveGameObjects;
	for (GameObject* ObjectToDelete : ObjectsToDelete)
	{
		TouhouEngine::Destroy(ObjectToDelete);
	}
}
Shader* TouhouEngine::GetNamedShader(std::string const& ShaderName)
{
	return(TouhouEngine::LoadedShaders[ShaderName]);
}
Texture* TouhouEngine::GetNamedTexture(std::string const& TextureName)
{
	Texture* ReturnValue = nullptr;
	if (LoadedTextures.find(TextureName) != LoadedTextures.end() )
	{
		ReturnValue = LoadedTextures[TextureName];
	}
	return(ReturnValue);
}
bool TouhouEngine::NamedTextureLoaded(std::string const& TextureName)
{
	return(LoadedTextures.find(TextureName) != LoadedTextures.end());
}
Texture* TouhouEngine::LoadNamedTexture(std::string const& TextureName, std::string const& ResourcePath)
{
	Texture* NewTexture = new Texture(ResourcePath, TextureName);
	LoadedTextures[TextureName] = NewTexture;
	return(NewTexture);
}
//Texture* TouhouEngine::LoadNamedTexture(std::string const& TextureName, std::string const& ResourcePath)
//{
//
//
//}
void DebugLogWrite(std::string TextToLog)
{

}

void TouhouEngine::PlaySound(std::string Path, float Volume)
{
	SoundEngine.PlaySound(Path, Volume);
}
void TouhouEngine::PlaySound(std::string Path, float RelativeVolume,std::string Tag)
{
	TouhouSoundEngineSoundObject* SoundToPlay = new TouhouSoundEngineSoundObject(Path, Tag);
	SoundToPlay->DynamicallyAllocatedAndTemporary = true;
	SoundToPlay->PlaySound(RelativeVolume);
}