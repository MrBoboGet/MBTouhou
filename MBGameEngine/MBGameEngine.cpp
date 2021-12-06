#include "MBGameEngine.h"
#include <filesystem>
namespace MBGameEngine
{

	//BEGIN Component
	MBGameEngine& Component::GetEngine()
	{
		return(m_AssociatedGameObject->GetEngine());
	}
	GameObjectReference Component::GetGameObject()
	{
		return(m_AssociatedGameObject);
	}
	const GameObjectReference Component::GetGameObject() const
	{
		return(m_AssociatedGameObject);
	}
	//GameObjectReference Component::GetGameObject() const
	//{
	//
	//}
	void Component::Update()
	{

	}
	Component::~Component()
	{

	}
	//END Component


	//BEGIN GameObject
	void GameObject::AddComponent(Component* ComponentToAdd)
	{
		m_Components.push_back(std::unique_ptr<Component>(ComponentToAdd));
		m_Components.back()->m_AssociatedGameObject = GameObjectReference(this, m_ObjectDeletedPointer);
	}
	void GameObject::p_DefaultUpdate()
	{
		for(std::unique_ptr<Component>& Components : m_Components)
		{
			if (*m_ObjectDeletedPointer == true)
			{
				break;
			}
			Components->Update();
		}
	}
	void GameObject::Update()
	{

	}
	void GameObject::OnCreate()
	{

	}
	//END GameObject

	//BEGIN ActiveObjectIterator
	bool ActiveObjectIterator::HasEnded()
	{
		return(m_HasEnded);
	}
	ActiveObjectIterator& ActiveObjectIterator::operator++()
	{
		return(Increment());
	}
	ActiveObjectIterator& ActiveObjectIterator::operator++(int)
	{
		return(Increment());
	}
	GameObjectReference& ActiveObjectIterator::operator*()
	{
		return(m_CurrentReference);
	}
	GameObjectReference& ActiveObjectIterator::operator->()
	{
		return(m_CurrentReference);
	}
	ActiveObjectIterator& ActiveObjectIterator::Increment()
	{
		while (m_InternalIterator != m_EndIterator)
		{
			m_InternalIterator++;
			if (m_InternalIterator == m_EndIterator)
			{
				break;
			}
			if (*m_InternalIterator != nullptr)
			{
				break;
			}
		}
		if (m_InternalIterator == m_EndIterator)
		{
			m_CurrentReference = GameObjectReference();
			m_HasEnded = true;
		}
		else
		{
			m_CurrentReference = GameObjectReference(m_InternalIterator->get(),(*m_InternalIterator)->m_ObjectDeletedPointer);
		}
		return(*this);
	}
	//END ActiveObjectIterator

	//BEGIN MBGameEngine
	void MBGameEngine::p_Update_CleanUpObjects()
	{
		m_ObjectsToDelete.clear();
		std::vector<std::unique_ptr<GameObject>> NewGameObjects = {};
		NewGameObjects.reserve(m_LoadedGameObjects.size()+m_NewObjects.size());
		for (size_t i = 0; i < m_LoadedGameObjects.size(); i++)
		{
			if (m_LoadedGameObjects[i] != nullptr)
			{
				NewGameObjects.push_back(std::move(m_LoadedGameObjects[i]));
			}
		}
		std::swap(m_LoadedGameObjects, NewGameObjects);
		for (size_t i = 0; i < m_NewObjects.size(); i++)
		{
			m_LoadedGameObjects.push_back(std::move(m_NewObjects[i]));
		}
		m_NewObjects.clear();
	}
	void MBGameEngine::p_Update_LoadedGameObjects()
	{
		for(std::unique_ptr<GameObject>& Object : m_LoadedGameObjects)
		{
			if (Object.get() == nullptr || Object->m_Active == false  || *Object->m_ObjectDeletedPointer == true || Object->Active == false)
			{
				continue;
			}
			Object->p_DefaultUpdate();
			if (Object.get() != nullptr)
			{
				Object->Update();
			}
		}
	}
	void MBGameEngine::p_Update()
	{
		p_Update_LoadedGameObjects();
		p_Update_CleanUpObjects();
		p_Update_Render();
	}
	bool MBGameEngine::GetKeyDown(KeyCode KeyToCheck)
	{
		return(m_InternalGraphicsEngine.GetKeyDown(KeyToCheck));
	}
	bool MBGameEngine::GetKeyPressed(KeyCode KeyToCheck)
	{
		return(m_InternalGraphicsEngine.GetKeyPressed(KeyToCheck));
	}
	bool MBGameEngine::GetKeyReleased(KeyCode KeyToCheck)
	{
		return(m_InternalGraphicsEngine.GetKeyReleased(KeyToCheck));
	}
	void MBGameEngine::Destroy(GameObject* ObjectToDelete)
	{
		//if (ObjectToDelete)
		//{
		//	m_ObjectsToDelete.push(ObjectToDelete);
		//	ObjectToDelete->m_Active = false;
		//}
		//inneffektivt, men att det fungerar är det viktigaste just nu
		if (ObjectToDelete == nullptr)
		{
			return;
		}
		for (size_t i = 0; i < m_LoadedGameObjects.size(); i++)
		{
			if (&(*ObjectToDelete) == m_LoadedGameObjects[i].get())
			{
				*m_LoadedGameObjects[i]->m_ObjectDeletedPointer = true;
				m_ObjectsToDelete.push_back(std::move(m_LoadedGameObjects[i]));
				break;
			}
		}
	}
	void MBGameEngine::Destroy(GameObjectReference ObjectToDelete)
	{
		if (ObjectToDelete == nullptr)
		{
			return;
		}
		for (size_t i = 0; i < m_LoadedGameObjects.size(); i++)
		{
			if (&(*ObjectToDelete) == m_LoadedGameObjects[i].get())
			{
				*m_LoadedGameObjects[i]->m_ObjectDeletedPointer = true;
				m_ObjectsToDelete.push_back(std::move(m_LoadedGameObjects[i]));
				break;
			}
		}
	}
	GameObjectReference MBGameEngine::Create(GameObject* NewObject)
	{
		m_NewObjects.push_back(std::unique_ptr<GameObject>(NewObject));
		NewObject->m_AssociatedEngine = this;
		NewObject->OnCreate();
		return(GameObjectReference(m_NewObjects.back().get(), m_NewObjects.back()->m_ObjectDeletedPointer));
	}
	void MBGameEngine::WindowCreate(float Width, float Height, std::string const& WindowName, bool FullScreen)
	{
		m_InternalGraphicsEngine.WindowCreate(Width, Height, WindowName, FullScreen);
		m_SpriteRenderingModel = std::unique_ptr<MBGE::SpriteModel>(new MBGE::SpriteModel(nullptr));
		m_SpriteCamera.SetOrtographicProjection(16, 9);
	}
	void MBGameEngine::MainLoop()
	{
		clock_t Timer = clock();
		clock_t DeltaTime = 0;
		size_t DisplayFpsCounter = 0;
		while (true)
		{
			while (((clock() - Timer) / double(CLOCKS_PER_SEC)) <= 1 / double(m_FrameRate))
			{}
			DisplayFpsCounter += 1;
			if (DisplayFpsCounter % 60 == 0)
			{
				std::cout << "DeltaTime:" << (clock() - Timer) / double(CLOCKS_PER_SEC) << "=" << double(CLOCKS_PER_SEC) / (clock() - Timer) << "fps" << std::endl;
			}
			Timer = clock();
			p_Update();
			m_InternalGraphicsEngine.Update();
			m_InternalGraphicsEngine.PollEvents();
		}
	}
	void MBGameEngine::p_Update_Render()
	{
		p_Update_Render_DrawDrawCalls();
	}
	void MBGameEngine::p_Update_Render_DrawDrawCalls()
	{
		std::sort(m_DrawCalls.begin(), m_DrawCalls.end());
		for (size_t i = 0; i < m_DrawCalls.size(); i++)
		{
			p_DrawDrawObject(m_DrawCalls[i]);
		}
		m_DrawCalls.clear();
	}
	void MBGameEngine::p_DrawDrawObject(DrawObject& ObjectToDraw)
	{
		if (ObjectToDraw.Texturen != nullptr)
		{
			ObjectToDraw.Texturen->Bind(0);
		}
		std::shared_ptr<Shader> ShaderToUse = nullptr;
		if (ObjectToDraw.Shader != nullptr)
		{
			ShaderToUse = ObjectToDraw.Shader;
		}
		else
		{
			//LEGACT GREJER, sprite shadern är default
			ShaderToUse = GetNamedShader("SpriteShader");
		}
		//MBGE::UniformValue ValuesToUse;
		m_SpriteCamera.WorldSpaceCoordinates = MBMath::MBVector3<float>(0, 0, -20);
		//TouhouEngine::__Camera.Update(ValuesToUse);
		ShaderToUse->Bind();
		m_SpriteRenderingModel->SetShader(ShaderToUse);
		m_SpriteRenderingModel->SetTexture(ObjectToDraw.Texturen);
		ShaderToUse->SetUniformMat4f("View", m_SpriteCamera.GetViewMatrix().GetContinousData());
		ShaderToUse->SetUniformMat4f("Projection", m_SpriteCamera.GetProjectionMatrix().GetContinousData());

		m_SpriteRenderingModel->ModelTransform = std::move(ObjectToDraw.TextureTransform);
		MBMath::MBVector3<float> NewScaling = m_SpriteRenderingModel->ModelTransform.GetScaling();
		NewScaling[0] *= ObjectToDraw.Width;
		NewScaling[1] *= ObjectToDraw.Height;
		m_SpriteRenderingModel->ModelTransform.SetScaling(NewScaling);
		ShaderToUse->SetUniformMat4f("Model", m_SpriteRenderingModel->ModelTransform.GetModelMatrix().GetContinousData());
		ObjectToDraw.UniformValues.UpdateUniforms("", ShaderToUse.get());
		m_SpriteRenderingModel->Draw();
		//LEGACY AF, SKA BORT
		if (ObjectToDraw.UniformValues.GetType() == MBGE::DataTypes::Struct)
		{
			if (ObjectToDraw.UniformValues.HasValue("ColorKoef"))
			{
				ShaderToUse->SetUniformVec4("ColorKoef", 1, 1, 1, 1);
			}
		}
	}
	void MBGameEngine::PlaySound(std::string const& SoundFile, float Volume)
	{

	}
	void MBGameEngine::PlaySound(std::string const&, float RelativeVolume, std::string const& Tag)
	{

	}

	ActiveObjectIterator MBGameEngine::GetActiveObjectsIterator()
	{
		ActiveObjectIterator ReturnValue;
		ReturnValue.m_InternalIterator = m_LoadedGameObjects.begin();
		ReturnValue.m_EndIterator = m_LoadedGameObjects.end();
		//ReturnValue.Increment();
		if (m_LoadedGameObjects.size() > 0)
		{
			ReturnValue.m_CurrentReference = m_LoadedGameObjects[0]->GetReference();
		}
		else
		{
			ReturnValue.m_HasEnded = true;
		}
		return(ReturnValue);
	}
	void MBGameEngine::ClearObjects()
	{
		ActiveObjectIterator Iterator = GetActiveObjectsIterator();
		while (Iterator.HasEnded() == false)
		{
			Destroy(*Iterator);
			Iterator++;
		}
	}
	std::shared_ptr<Shader> MBGameEngine::GetNamedShader(std::string const& ShaderName)
	{
		std::shared_ptr<Shader> ReturnValue;
		if (m_LoadedShaders.find(ShaderName) != m_LoadedShaders.end())
		{
			ReturnValue = m_LoadedShaders[ShaderName];
		}
		return(ReturnValue);
	}
	std::shared_ptr<Texture> MBGameEngine::GetNamedTexture(std::string const& TextureName)
	{
		std::shared_ptr<Texture> ReturnValue;
		if (m_LoadedTextures.find(TextureName) != m_LoadedTextures.end())
		{
			ReturnValue = m_LoadedTextures[TextureName];
		}
		else
		{
			//LEGACY KOD, ska bort
			if (std::filesystem::exists("./Resources/SpelResurser/Sprites/" + TextureName))
			{
				ReturnValue =LoadNamedTexture(TextureName, "./Resources/SpelResurser/Sprites/" + TextureName);
			}
		}
		return(ReturnValue);
	}
	std::shared_ptr<Texture> MBGameEngine::LoadNamedTexture(std::string const& TextureName, std::string const& ResourcePath)
	{
		if (NamedTextureLoaded(TextureName))
		{
			return(m_LoadedTextures[TextureName]);
		}
		std::shared_ptr<Texture> NewTexture = std::shared_ptr<Texture>(new Texture(ResourcePath));
		m_LoadedTextures[TextureName] = NewTexture;
		return(NewTexture);
	}
	std::shared_ptr<Texture> MBGameEngine::LoadNamedTexture(std::string const& TextureName, std::string const& ResourcePath, uint64_t Options)
	{
		if (NamedTextureLoaded(TextureName))
		{
			return(m_LoadedTextures[TextureName]);
		}
		std::shared_ptr<Texture> NewTexture = std::shared_ptr<Texture>(new Texture(ResourcePath,Options));
		m_LoadedTextures[TextureName] = NewTexture;
		return(NewTexture);
	}
	std::shared_ptr<Shader> MBGameEngine::LoadShader(std::string const& ShaderName, std::string const& VertexFilepath, std::string const& FragmentFilepath)
	{
		std::shared_ptr<Shader> NewShader = std::shared_ptr<Shader>(new MBGE::ShaderProgram(VertexFilepath, FragmentFilepath));
		m_LoadedShaders[ShaderName] = NewShader;
		return(NewShader);
	}
	bool MBGameEngine::NamedTextureLoaded(std::string const& TextureName)
	{
		return(m_LoadedTextures.find(TextureName) != m_LoadedTextures.end());
	}
	void MBGameEngine::DrawTexture(std::string const& NamedTexture, MBGE::Transform TextureTransform, float Width, float Height, std::array<int, 4> Layer)
	{
		DrawTexture(GetNamedTexture(NamedTexture), TextureTransform, Width, Height, Layer);
	}
	void MBGameEngine::DrawTexture(std::shared_ptr<Texture> TextureToDraw, MBGE::Transform TextureTransform, float Width, float Height, std::array<int, 4> Layer)
	{
		DrawObject NewDrawObject;
		NewDrawObject.Texturen = TextureToDraw;
		NewDrawObject.TextureTransform = TextureTransform;
		NewDrawObject.Width = Width;
		NewDrawObject.Height = Height;
		NewDrawObject.Layer = Layer;
		m_DrawCalls.push_back(NewDrawObject);
	}
	void MBGameEngine::DrawTexture(std::shared_ptr<Texture> TextureToDraw, std::shared_ptr<Shader> ShaderToUse, MBGE::UniformValue InformValues, MBGE::Transform const& TextureTransform, float Width, float Height, std::array<int, 4> const& Layer)
	{
		DrawObject NewDrawObject;
		NewDrawObject.Shader = ShaderToUse;
		NewDrawObject.UniformValues = std::move(InformValues);
		NewDrawObject.Texturen = TextureToDraw;
		NewDrawObject.TextureTransform = TextureTransform;
		NewDrawObject.Width = Width;
		NewDrawObject.Height = Height;
		NewDrawObject.Layer = Layer;
		m_DrawCalls.push_back(NewDrawObject);
	}
	GameObjectReference MBGameEngine::FindObjectWithName(std::string const& Name)
	{
		GameObjectReference ReturnValue;
		for (std::unique_ptr<GameObject>& Objects : m_LoadedGameObjects)
		{
			if (Objects.get() != nullptr && Objects->GetName() == Name && Objects->m_Active)
			{
				ReturnValue = GameObjectReference(Objects.get(),Objects->m_ObjectDeletedPointer);
				break;
			}
		}
		return(ReturnValue);
	}
	GameObjectReference MBGameEngine::FindObjectWithTag(std::string const& Name)
	{
		GameObjectReference ReturnValue;
		for (std::unique_ptr<GameObject>& Objects : m_LoadedGameObjects)
		{
			if (Objects.get() != nullptr && Objects->GetTag() == Name && Objects->m_Active)
			{
				ReturnValue = GameObjectReference(Objects.get(),Objects->m_ObjectDeletedPointer);
				break;
			}
		}
		return(ReturnValue);
	}
	//END MBGameEngine
}