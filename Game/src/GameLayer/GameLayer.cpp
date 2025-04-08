#include "GameLayer.h"

GameLayer::GameLayer() : Layer("LevelEditor")
{
	_Window = std::make_shared<EngineWindow>(EngineApp::Get().GetWindow());
	_Window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	_Shader = EngineCreateShader("../Engine/Shaders/base_vertex.glsl", "../Engine/Shaders/base_fragment.glsl");
	_ShadowShader = EngineCreateShader("../Engine/Shaders/shadow_vertex.glsl", "../Engine/Shaders/shadow_fragment.glsl");
	_LoadedScene = std::make_shared<EngineScene>();

	_LoadedScene->CreateEntity(_Camera, "Camera");
	_LoadedScene->GetRegistry().emplace<Engine::CameraComponent>(_Camera);

	EngineLoadScene("main.scene", *_LoadedScene);
	_RenderSystem = std::make_shared<EngineRenderSystem>(_Shader, _ShadowShader, _Window, _LoadedScene->GetRegistry());
}

void GameLayer::OnUpdate()
{
	if (EngineKeyPressed(E_KEY_W))
		GetActiveCamera()->ProcessKeyboard(Engine::ECameraDirection::FORWARD, 0.1f);
	if (EngineKeyPressed(E_KEY_S))
		GetActiveCamera()->ProcessKeyboard(Engine::ECameraDirection::BACKWARD, 0.1f);
	if (EngineKeyPressed(E_KEY_A))
		GetActiveCamera()->ProcessKeyboard(Engine::ECameraDirection::LEFT, 0.1f);
	if (EngineKeyPressed(E_KEY_D))
		GetActiveCamera()->ProcessKeyboard(Engine::ECameraDirection::RIGHT, 0.1f);

	if (GetActiveCamera() != NULL) {
		EngineSetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		EngineClear();

		RendererBeginScene(*GetActiveCamera());

		_RenderSystem->Draw();

		RendererEndScene();
	}
}

bool GameLayer::OnMouseMove(Engine::MouseMovedEvent& e)
{
	if (_bfirstPressed)
	{
		_LastX = e.GetX();
		_LastY = e.GetY();
		_bfirstPressed = false;
		return false;
	}

	float xOffset = e.GetX() - _LastX;
	float yOffset = _LastY - e.GetY();

	_LastX = e.GetX();
	_LastY = e.GetY();

	GetActiveCamera()->ProcessMouseMovement(xOffset, yOffset);
	return false;
}

Engine::CameraComponent* GameLayer::GetActiveCamera()
{
	if (_LoadedScene->GetRegistry().valid(_Camera) && _LoadedScene->GetRegistry().any_of<Engine::CameraComponent>(_Camera)) {
		auto& camera = _LoadedScene->GetRegistry().get<Engine::CameraComponent>(_Camera);
		return &camera;
	}
	else {
		std::cout << "No Active Camera" << std::endl;
	}
}

void GameLayer::SetEditor(EngineLayer* leveleditor, EngineLayer* editorUI)
{
	_levelEditor.reset(leveleditor);
	_levelEditorUI.reset(editorUI);
}
