#include <Engine.h>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <filesystem>
#include <assert.h>
//std::unordered_map<std::string, std::shared_ptr<Shader>> TouhouEngine::LoadedShaders = {};
//std::unordered_map<std::string, std::shared_ptr<Texture>> TouhouEngine::LoadedTextures = {};
//GLFWwindow* TouhouEngine::CurrentWindow = nullptr;
//
//std::vector<GameObject*> TouhouEngine::ActiveGameObjects = {};
//std::vector<GameObject*> TouhouEngine::DeletedGameObjects = {};
//std::vector<DrawObject> TouhouEngine::DrawCalls = {};
//float TouhouEngine::Framerate = 0.0166666f;
//bool TouhouEngine::UpdatePause = false;
//
//std::vector<std::string> TouhouEngine::KeyList = std::vector<std::string>(0);
//std::unordered_map<std::string, bool> TouhouEngine::PressedKeys = std::unordered_map<std::string, bool>(0);
//std::unordered_map<std::string, bool> TouhouEngine::PreviouslyPressedKeys = std::unordered_map<std::string, bool>(0);
//
//TouhouSoundEngine TouhouEngine::SoundEngine = TouhouSoundEngine();
//std::vector<TouhouSoundEngineSoundObject*> TouhouEngine::SoundObjects = std::vector<TouhouSoundEngineSoundObject*>(0);
//MBGE::Camera TouhouEngine::__Camera = MBGE::Camera();
//std::unique_ptr<MBGE::SpriteModel> TouhouEngine::__SpriteModel = std::unique_ptr<MBGE::SpriteModel>(nullptr);
////std::vector<void(*)()> TouhouEngine::CustomUpdateProcedures = std::vector<void(*)()>(0);
//#define GLCall(x) x
//TouhouEngine::TouhouEngine()
//{
//}
//
//TouhouEngine::~TouhouEngine()
//{
//}
//GameObject* TouhouEngine::Create(GameObject* a)
//{
//	ActiveGameObjects.push_back(a);
//	a->OnCreate();
//	return(a);
//}
//void TouhouEngine::Destroy(GameObject* a)
//{
//	int PositionIListan = -1;
//	for (int i = 0; i < ActiveGameObjects.size();i++)
//	{
//		if (ActiveGameObjects[i] == a)
//		{
//			PositionIListan = i;
//			break;
//		}
//	}
//	if (PositionIListan == -1)
//	{
//		//dubble delet ska inett delata randomly
//		return;
//	}
//	//OBS!!! Vi måte erasa efter vi deletat grejer, annars så kommer vi få fel
//	ActiveGameObjects[PositionIListan]->Active = false;
//	DeletedGameObjects.push_back(ActiveGameObjects[PositionIListan]);
//	assert(DeletedGameObjects.back() == a);
//	ActiveGameObjects.erase(ActiveGameObjects.begin() + PositionIListan);
//}
//GameObject* TouhouEngine::FindObjectWithName(std::string Namn)
//{
//	//just nu har vi inte ett bättre system än att vi går igenom objekten i tur och ordning till vi hittar ett föremål med det namnet. Vi kanske vill ha en hashlista i framtiden
//	for (int i = 0; i < ActiveGameObjects.size(); i++)
//	{
//		if (ActiveGameObjects[i]->GetName() == Namn)
//		{
//			return(ActiveGameObjects[i]);
//		}
//	}
//	//nu har alltså inget namn hittats, detta borde egentligen krascha programmet
//	return(nullptr);
//}
//GameObject* TouhouEngine::FindObjectWithTag(std::string const& Tag)
//{
//	//just nu har vi inte ett bättre system än att vi går igenom objekten i tur och ordning till vi hittar ett föremål med det namnet. Vi kanske vill ha en hashlista i framtiden
//	for (int i = 0; i < ActiveGameObjects.size(); i++)
//	{
//		if (ActiveGameObjects[i]->GetTag() == Tag)
//		{
//			return(ActiveGameObjects[i]);
//		}
//	}
//	//nu har alltså inget namn hittats, detta borde egentligen krascha programmet
//	return(nullptr);
//}
//std::shared_ptr<Shader> TouhouEngine::LoadShader(std::string ShaderName, std::string VertexFilepath, std::string FragmentFilepath)
//{
//	std::shared_ptr<Shader> NyaShadern = std::shared_ptr<Shader>(new Shader("Resources/Shaders/"+VertexFilepath, "Resources/Shaders/"+FragmentFilepath));
//	LoadedShaders[ShaderName] = NyaShadern;
//	return(NyaShadern);
//}
//void TouhouEngine::UpdateGameobjects(std::vector<GameObject*> GameobjectsToUpdate)
//{
//	for (int i = 0; i < GameobjectsToUpdate.size();i++)
//	{
//		if (GameobjectsToUpdate[i]->Active)
//		{
//			GameobjectsToUpdate[i]->UpdateComponents();
//			GameobjectsToUpdate[i]->Update();
//		}
//	}
//}
//bool LayerIsBigger(std::vector<int> Layer1, std::vector<int> Layer2)
//{
//	bool IsBigger = false;
//	for (int i = 0; i < Layer1.size();i++)
//	{
//		if (i < Layer2.size())
//		{
//			if (Layer1[i] > Layer2[i])
//			{
//				IsBigger = true;
//				break;
//			}
//		}
//		else
//		{
//			break;
//		}
//	}
//	return(IsBigger);
//}
//void TouhouEngine::Render(std::vector<GameObject*> Lista)
//{
//	//Sorterar draw calls, vi kan använda standard sort istället
//
//	//if (TouhouEngine::DrawCalls.size() > 1)
//	//{
//	//	for (int i = 0;i < TouhouEngine::DrawCalls.size() - 1; i++)
//	//	{
//	//		for (int j = 0; j < TouhouEngine::DrawCalls.size() - 1 - i; j++)
//	//		{
//	//			//själva jämförelsen, bygger på hur Layer systemet fungerar
//	//			if (LayerIsBigger(TouhouEngine::DrawCalls[j].Layer, TouhouEngine::DrawCalls[j + 1].Layer) == true)
//	//			{
//	//				//Då byter vi plats på dem
//	//				auto Temp = std::move(TouhouEngine::DrawCalls[j]);
//	//				TouhouEngine::DrawCalls[j] = std::move(TouhouEngine::DrawCalls[j + 1]);
//	//				TouhouEngine::DrawCalls[j + 1] = std::move(Temp);
//	//			}
//	//		}
//	//	}
//	//}
//	std::sort(TouhouEngine::DrawCalls.begin(), TouhouEngine::DrawCalls.end());
//	for (size_t i = 0; i < TouhouEngine::DrawCalls.size(); i++)
//	{
//		p_DrawDrawObject(TouhouEngine::DrawCalls[i]);
//	}
//	TouhouEngine::DrawCalls = {};
//}
//void TouhouEngine::InitializeEngineCompononets()
//{
//	SoundEngine.Initialize();
//	TouhouEngine::__Camera.SetOrtographicProjection(16, 9);
//}
//void TouhouEngine::UpdateSoundSystem()
//{
//	SoundEngine.Update();
//}
//void TouhouEngine::CleanupFinishedSounds()
//{
//	//int SizeOfSoundObjects = SoundObjects.size();
//	//std::vector<int> IndexOfSoundsToRemove = std::vector<int>(0);
//	//for (size_t i = 0; i < SizeOfSoundObjects; i++)
//	//{
//	//	if (SoundObjects[i]->DynamicallyAllocatedAndTemporary)
//	//	{
//	//		if (!SoundObjects[i]->IsPlaying())
//	//		{
//	//			IndexOfSoundsToRemove.push_back(i);
//	//		}
//	//	}
//	//}
//	//int SizeOfSoundToRemove = IndexOfSoundsToRemove.size();
//	//for (size_t i = 0; i < SizeOfSoundToRemove; i++)
//	//{
//	//	delete SoundObjects[i];
//	//	SoundObjects.erase(SoundObjects.begin() + IndexOfSoundsToRemove[i] - i);
//	//}
//}
//void TouhouEngine::Mainloop()
//{
//	clock_t Timer = clock();
//	int FrameCount = 0;
//	InitializeEngineCompononets();
//	while (!glfwWindowShouldClose(TouhouEngine::CurrentWindow))
//	{
//		if (float(clock() - Timer) / CLOCKS_PER_SEC > Framerate)
//		{
//			if (float(clock() - Timer) / CLOCKS_PER_SEC > Framerate + 0.01)
//			{
//				std::cout << "Fps is not exactly 60" << std::endl;
//			}
//			glClear(GL_COLOR_BUFFER_BIT);
//			FrameCount += 1;
//			Timer = clock();
//			UpdateSoundSystem();
//			HandleDeletedGameobjects();
//			std::vector<GameObject*> CurrentlyActiveGameObjects = ActiveGameObjects;
//			clock_t Timer2 = clock();
//			UpdateKeys();
//			
//			if (!UpdatePause)
//			{
//				UpdateGameobjects(CurrentlyActiveGameObjects);
//			}
//			if (FrameCount % 60 == 0)
//			{
//				std::cout << "Update Time is " << float(clock() - Timer2) / CLOCKS_PER_SEC << std::endl;
//			}
//			Timer2 = clock();
//			Render(CurrentlyActiveGameObjects);
//			if (FrameCount % 60 == 0)
//			{
//				std::cout << "Render time is " << float(clock() - Timer2) / CLOCKS_PER_SEC << std::endl;
//			}
//			glfwSwapBuffers(TouhouEngine::CurrentWindow);
//		}
//		glfwPollEvents();
//	}
//}
//void TouhouEngine::HandleDeletedGameobjects()
//{
//	//nu deletar vi alla föremål som vi ville ha bort förra framen
//	for (int i = 0; i < TouhouEngine::DeletedGameObjects.size();i++)
//	{
//		delete TouhouEngine::DeletedGameObjects[i];
//	}
//	TouhouEngine::DeletedGameObjects = {};
//}
//void TouhouEngine::InitializeWindow(int Width, int Height, std::string WindowName, GLFWmonitor* Monitor, GLFWwindow* Share)
//{
//	TouhouEngine::CurrentWindow = glfwCreateWindow(Width, Height, WindowName.c_str(), NULL, Share);
//	if (!TouhouEngine::CurrentWindow)
//	{
//		glfwTerminate();
//		assert(false);
//	}
//	/* Make the window's context current */
//	glfwMakeContextCurrent(TouhouEngine::CurrentWindow);
//	if (gl3wInit() == GL3W_OK)
//	{
//		std::cout << "det funkar" << std::endl;
//	}
//	//ser till att det fins blending, ändra inte vet inte hur den funkar
//	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
//	GLCall(glEnable(GL_BLEND));
//	//den här koden gör så att v-sync är på
//	glfwSwapInterval(1);
//
//	//för att göra grejer
//	TouhouEngine::__SpriteModel = std::unique_ptr<MBGE::SpriteModel>(new MBGE::SpriteModel(nullptr));
//}
//
////keyPresses
//
//std::unordered_map<std::string, int> GLFWKeycodes = std::unordered_map<std::string, int>(0);
//int CharacterToGLFWKey(std::string Character)
//{
//	if (GLFWKeycodes.find(Character) != GLFWKeycodes.end())
//	{
//		return(GLFWKeycodes[Character]);
//	}
//	else
//	{
//		//innebnär att det inte är en prinbar key, vi har ej support för det än så länge
//		return(-1);
//	}
//}
//bool InitializedKeys = false;
//bool MapGenerated = false;
//void TouhouEngine::UpdateKeys()
//{
//	if (!MapGenerated)
//	{
//		//generar en map för alla printable keys
//		for (size_t i = 1; i < 348; i++)
//		{
//			if (glfwGetKeyName(i, 0) != NULL)
//			{
//				GLFWKeycodes[glfwGetKeyName(i, 0)] = i;
//			}
//		}
//		//lite hardcodace värden
//		GLFWKeycodes["space"] = 32;
//		GLFWKeycodes["esc"] = 256;
//		GLFWKeycodes["tab"] = 258;
//		GLFWKeycodes["left"] = 263;
//		GLFWKeycodes["right"] = 262;
//		GLFWKeycodes["up"] = 265;
//		GLFWKeycodes["down"] = 264;
//		GLFWKeycodes["leftshift"] = 340;
//		GLFWKeycodes["enter"] = 257;
//		MapGenerated = true;
//	}
//	if (!InitializedKeys)
//	{
//		KeyList.reserve(GLFWKeycodes.size());
//		for (auto kv : GLFWKeycodes) 
//		{
//			KeyList.push_back(kv.first);
//		}
//		InitializedKeys = true;
//		//uppdaterar våra values, men sätter prevuious direkt till falsk
//		for (size_t i = 0; i < KeyList.size(); i++)
//		{
//			PreviouslyPressedKeys[KeyList[i]] = false;
//			PressedKeys[KeyList[i]] = false;
//		}
//	}
//	else
//	{
//		PreviouslyPressedKeys = PressedKeys;
//		for (size_t i = 0; i < KeyList.size(); i++)
//		{
//			PressedKeys[KeyList[i]] = glfwGetKey(CurrentWindow,GLFWKeycodes[KeyList[i]]);
//		}
//	}
//}
//bool TouhouEngine::GetKeyDown(std::string Key)
//{
//	return(PressedKeys[Key]);
//}
//bool TouhouEngine::GetKeyPressed(std::string Key)
//{
//	if(PressedKeys[Key] == true && PreviouslyPressedKeys[Key] == false)
//	{
//		return(true);
//	}
//	else
//	{
//		return(false);
//	}
//}
//bool TouhouEngine::GetKeyReleased(std::string Key)
//{
//	if (PressedKeys[Key] == false && PreviouslyPressedKeys[Key] == true)
//	{
//		return(true);
//	}
//	else
//	{
//		return(false);
//	}
//}
////void TouhouEngine::_GetWindowSize(int* Width, int* Height)
////{
////	//TouhouEngine::__GraphicsEngine.GetWindowSize(Width,Height);
////	glfwGetWindowSize(TouhouEngine::CurrentWindow, Width, Height);
////}
//ActiveObjectIterator TouhouEngine::GetActiveObjectsIterator()
//{
//	ActiveObjectIterator ReturnValue;
//	ReturnValue.m_InternalIterator = TouhouEngine::ActiveGameObjects.begin();
//	ReturnValue.m_EndIterator = TouhouEngine::ActiveGameObjects.end();
//	return(ReturnValue);
//}
//void TouhouEngine::ClearObjects()
//{
//	std::vector<GameObject*> ObjectsToDelete = TouhouEngine::ActiveGameObjects;
//	for (GameObject* ObjectToDelete : ObjectsToDelete)
//	{
//		TouhouEngine::Destroy(ObjectToDelete);
//	}
//}
//std::shared_ptr<Shader> TouhouEngine::GetNamedShader(std::string const& ShaderName)
//{
//	return(TouhouEngine::LoadedShaders[ShaderName]);
//}
//std::shared_ptr<Texture> TouhouEngine::GetNamedTexture(std::string const& TextureName)
//{
//	std::shared_ptr<Texture> ReturnValue = nullptr;
//	if (LoadedTextures.find(TextureName) != LoadedTextures.end() )
//	{
//		ReturnValue = LoadedTextures[TextureName];
//	}
//	else
//	{
//		//LEGACY KOD, ska bort
//		if (std::filesystem::exists("./Resources/SpelResurser/Sprites/" + TextureName))
//		{
//			ReturnValue = TouhouEngine::LoadNamedTexture(TextureName, "./Resources/SpelResurser/Sprites/" + TextureName);
//		}
//	}
//	return(ReturnValue);
//}
//bool TouhouEngine::NamedTextureLoaded(std::string const& TextureName)
//{
//	return(LoadedTextures.find(TextureName) != LoadedTextures.end());
//}
//std::shared_ptr<Texture> TouhouEngine::LoadNamedTexture(std::string const& TextureName, std::string const& ResourcePath)
//{
//	std::shared_ptr<Texture> ReturnValue = nullptr;
//	if (LoadedTextures.find(TextureName) == LoadedTextures.end())
//	{
//		std::shared_ptr<Texture> NewTexture = std::shared_ptr<Texture>(new Texture(ResourcePath));
//		LoadedTextures[TextureName] = NewTexture;
//		ReturnValue = NewTexture;
//	}
//	else
//	{
//		ReturnValue = LoadedTextures[TextureName];
//	}
//	return(ReturnValue);
//}
//void TouhouEngine::DrawTexture(std::string const& NamedTexture, MBGE::Transform TextureTransform, float Width, float Height, std::array<int,4> Layer)
//{
//	if (NamedTexture.find("Back") != NamedTexture.npos)
//	{
//		std::cout << "";
//	}
//	DrawTexture(TouhouEngine::GetNamedTexture(NamedTexture), TextureTransform, Width, Height, Layer);
//}
//void TouhouEngine::DrawTexture(std::shared_ptr<Texture> TextureToDraw, MBGE::Transform TextureTransform, float Width, float Height, std::array<int,4>Layer)
//{
//	if (TextureToDraw == nullptr)
//	{
//		std::cout << "";
//	}
//	DrawObject NewObjectToDraw;
//	NewObjectToDraw.Texturen = TextureToDraw;
//	NewObjectToDraw.TextureTransform = std::move(TextureTransform);
//	NewObjectToDraw.Width = Width;
//	NewObjectToDraw.Height = Height;
//	for (size_t i = 0; i < 4; i++)
//	{
//		NewObjectToDraw.Layer[i] = Layer[i];
//	}
//	TouhouEngine::DrawCalls.push_back(std::move(NewObjectToDraw));
//}
//void TouhouEngine::p_DrawDrawObject(DrawObject& ObjectToDraw)
//{
//	if (ObjectToDraw.Texturen == nullptr)
//	{
//		return;
//	}
//	//ObjectToDraw.Position = Vector2D(0, 0);
//	//ObjectToDraw.Width = 8;
//	//ObjectToDraw.Height = 5;
//	ObjectToDraw.Texturen->Bind(0);
//	//MBGE::UniformValue ValuesToUse;
//	TouhouEngine::__Camera.WorldSpaceCoordinates = MBMath::MBVector3<float>(0, 0, -20);
//	//TouhouEngine::__Camera.Update(ValuesToUse);
//	auto ShaderToUse = GetNamedShader("SpriteShader");
//	TouhouEngine::__SpriteModel->SetShader(ShaderToUse);
//	TouhouEngine::__SpriteModel->SetTexture(ObjectToDraw.Texturen);
//	ShaderToUse->SetUniformMat4f("View", TouhouEngine::__Camera.GetViewMatrix().GetContinousData());
//	ShaderToUse->SetUniformMat4f("Projection", TouhouEngine::__Camera.GetProjectionMatrix().GetContinousData());
//
//	TouhouEngine::__SpriteModel->ModelTransform = std::move(ObjectToDraw.TextureTransform);
//	MBMath::MBVector3<float> NewScaling = TouhouEngine::__SpriteModel->ModelTransform.GetScaling();
//	NewScaling[0] *= ObjectToDraw.Width;
//	NewScaling[1] *= ObjectToDraw.Height;
//	TouhouEngine::__SpriteModel->ModelTransform.SetScaling(NewScaling);
//	//TouhouEngine::__SpriteModel->ModelTransform.SetScaling(MBMath::MBVector3<float>(ObjectToDraw.Width,ObjectToDraw.Height,1));
//	//TouhouEngine::__SpriteModel->ModelTransform.SetPosition(MBMath::MBVector3<float>(ObjectToDraw.Position.x,ObjectToDraw.Position.y,0));
//
//
//	ShaderToUse->SetUniformMat4f("Model",TouhouEngine::__SpriteModel->ModelTransform.GetModelMatrix().GetContinousData());
//	//if (ObjectToDraw.Height == 36)
//	//{
//	//	std::cout << "";
//	//	MBMath::MBMatrix4<float> View = ValuesToUse.GetValue("View").GetMat4();
//	//	MBMath::MBMatrix4<float> Projection = ValuesToUse.GetValue("Projection").GetMat4();
//	//	MBMath::MBMatrix4<float> Model = TouhouEngine::__SpriteModel->ModelTransform.GetModelMatrix();
//	//	std::cout << "View:" << std::endl << View<<std::endl;
//	//	std::cout << "Projection:" << std::endl << Projection <<std::endl;
//	//	std::cout << "Model:" << std::endl << Model <<std::endl;
//	//	MBMath::MBStaticVector<float,4> TestVector1;
//	//	TestVector1[0] = -0.5;
//	//	TestVector1[1] = -0.5;
//	//	TestVector1[2] = 0;
//	//	TestVector1[3] = 1;
//	//	std::cout << Projection * View * Model * TestVector1 <<std::endl;
//	//	MBMath::MBStaticVector<float, 4> TestVector2;
//	//	TestVector2[0] = -0.5;
//	//	TestVector2[1] = 0.5;
//	//	TestVector2[2] = 0;
//	//	TestVector2[3] = 1;
//	//	std::cout << Projection * View * Model * TestVector2 << std::endl;
//	//	MBMath::MBStaticVector<float, 4> TestVector3;
//	//	TestVector3[0] = 0.5;
//	//	TestVector3[1] = 0.5;
//	//	TestVector3[2] = 0;
//	//	TestVector3[3] = 1;
//	//	std::cout << Projection * View * Model * TestVector3 << std::endl;
//	//	MBMath::MBStaticVector<float, 4> TestVector4;
//	//	TestVector4[0] = 0.5;
//	//	TestVector4[1] = -0.5;
//	//	TestVector4[2] = 0;
//	//	TestVector4[3] = 1;
//	//	std::cout << Projection * View * Model * TestVector4 << std::endl;
//	//	std::cout << "Position: x=" << ObjectToDraw.Position.x << " y=" << ObjectToDraw.Position.y << std::endl;
//	//	ShaderToUse->PrintActiveAttributesAndUniforms();
//	//}
//	TouhouEngine::__SpriteModel->Draw();
//}
////Texture* TouhouEngine::LoadNamedTexture(std::string const& TextureName, std::string const& ResourcePath)
////{
////
////
////}
//void DebugLogWrite(std::string TextToLog)
//{
//
//}
//
//void TouhouEngine::PlaySound(std::string Path, float Volume)
//{
//	SoundEngine.PlaySound(Path, Volume);
//}
//void TouhouEngine::PlaySound(std::string Path, float RelativeVolume,std::string Tag)
//{
//	TouhouSoundEngineSoundObject* SoundToPlay = new TouhouSoundEngineSoundObject(Path, Tag);
//	SoundToPlay->DynamicallyAllocatedAndTemporary = true;
//	SoundToPlay->PlaySound(RelativeVolume);
//}