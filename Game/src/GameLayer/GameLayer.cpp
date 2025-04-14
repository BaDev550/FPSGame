#include "GameLayer.h"
#include <memory>

float g_DeltaTime = 0.0f;
float g_LastFrameTime = 0.0f;

GameLayer::GameLayer() : Layer("LevelEditor")
{
	_Window = std::make_shared<EngineWindow>(EngineApp::Get().GetWindow());
	_Window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	_Shader = EngineCreateShader("../Engine/Shaders/base_vertex.glsl", "../Engine/Shaders/base_fragment.glsl");
	_LoadedScene = std::make_shared<EngineScene>();

	//EngineLoadScene("main.scene", *_LoadedScene);

	_RenderSystem = std::make_shared<EngineRenderSystem>(_Shader);
	_PhysicsSystem = std::make_shared<EnginePhysicsSystem>();

	Player = std::make_shared<PlayerPawn>();
	for (size_t i = 0; i < 50; i++) {
		std::string strName = "Entity" + std::to_string(i);
		EngineEntity TempEntity(_LoadedScene->CreateEntity(strName));

		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> distX(-10, 14);
		std::uniform_int_distribution<int> distZ(-26, 9);

		float x = static_cast<float>(i + distX(rng));
		float y = 0.0f;
		float z = static_cast<float>(i + distZ(rng));

		TempEntity.GetComponent<Engine::TransformComponent>().SetPosition(glm::vec3(x, y, z));
		TempEntity.AddComponent<Engine::MeshComponent>("Assets/Models/mario_2/mario_2.obj");
	}
	EngineScene::Get().Start();
}

void GameLayer::OnUpdate()
{
	float currentTime = (float)glfwGetTime();
	g_DeltaTime = currentTime - g_LastFrameTime;
	g_LastFrameTime = currentTime;

	if (!_bLevelEditorOpened) {
		EngineScene::Get().Update();
		_PhysicsSystem->Update(g_DeltaTime);
	}

	OnRender();
}

void GameLayer::OnRender()
{
	if (GetActiveCamera() != NULL) {
		_RenderSystem->Draw(*GetActiveCamera());
	}
	else {
		std::cout << "No Active Camera not rendering" << std::endl;
	}
}

Engine::CameraComponent* GameLayer::GetActiveCamera()
{
	if (_bLevelEditorOpened) {
		return _levelEditor->GetActiveCamera();
	}
	else {
		if (Player->IsValid() && Player->GetCameraEntity()->IsValid()) {
			return &Player->GetCameraEntity()->GetCamera();
		}
		else {
			return _levelEditor->GetActiveCamera();
		}
	}
}

void GameLayer::SetEditor(LEditor* leveleditor, LEditorUI* editorUI)
{
	_levelEditor.reset(leveleditor);
	_levelEditorUI.reset(editorUI);
}
