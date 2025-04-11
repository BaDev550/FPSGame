#include "GameLayer.h"

GameLayer::GameLayer() : Layer("LevelEditor")
{
	_Window = std::make_shared<EngineWindow>(EngineApp::Get().GetWindow());
	_Window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	_Shader = EngineCreateShader("../Engine/Shaders/base_vertex.glsl", "../Engine/Shaders/base_fragment.glsl");
	_LoadedScene = std::make_shared<EngineScene>();

	EngineLoadScene("main.scene", *_LoadedScene);
	_LoadedScene->CreateCamera(_Camera, "GameCamera");

	_RenderSystem = std::make_shared<EngineRenderSystem>(_Shader, _LoadedScene->GetRegistry());
}

void GameLayer::OnUpdate()
{
	if (EngineKeyPressed(E_KEY_W) && !_bLevelEditorOpened)
		GetActiveCamera()->ProcessKeyboard(Engine::ECameraDirection::FORWARD, 0.1f);
	if (EngineKeyPressed(E_KEY_S) && !_bLevelEditorOpened)
		GetActiveCamera()->ProcessKeyboard(Engine::ECameraDirection::BACKWARD, 0.1f);
	if (EngineKeyPressed(E_KEY_A) && !_bLevelEditorOpened)
		GetActiveCamera()->ProcessKeyboard(Engine::ECameraDirection::LEFT, 0.1f);
	if (EngineKeyPressed(E_KEY_D) && !_bLevelEditorOpened)
		GetActiveCamera()->ProcessKeyboard(Engine::ECameraDirection::RIGHT, 0.1f);

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

bool GameLayer::OnMouseMove(Engine::MouseMovedEvent& e)
{
	if (_bLevelEditorOpened || GetActiveCamera() == NULL) return false;

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
	if (_bLevelEditorOpened) {
		return _levelEditor->GetActiveCamera();
	}
	else {
		if (_LoadedScene->GetRegistry().valid(_Camera) && _LoadedScene->GetRegistry().any_of<Engine::CameraComponent>(_Camera)) {
			auto& camera = _LoadedScene->GetRegistry().get<Engine::CameraComponent>(_Camera);
			return &camera;
		}
		else {
			std::cout << "No Active Camera" << std::endl;
		}
	}
}

void GameLayer::SetEditor(LEditor* leveleditor, LEditorUI* editorUI)
{
	_levelEditor.reset(leveleditor);
	_levelEditorUI.reset(editorUI);
}
