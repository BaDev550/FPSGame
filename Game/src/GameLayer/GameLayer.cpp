#include "GameLayer.h"
#include <memory>

GameLayer::GameLayer() : Layer("LevelEditor")
{
	_Window = std::make_shared<EngineWindow>(EngineApp::Get().GetWindow());
	_Window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	_Shader = EngineCreateShader("../Engine/Shaders/base_vertex.glsl", "../Engine/Shaders/base_fragment.glsl");
	_LoadedScene = std::make_shared<EngineScene>();

	EngineLoadScene("main.scene", *_LoadedScene);

	_RenderSystem = std::make_shared<EngineRenderSystem>(_Shader);

	Player = std::make_shared<PlayerPawn>();
	EngineScene::Get().Start();
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

	if(!_bLevelEditorOpened)
		EngineScene::Get().Update();

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
		return Player->GetCamera();
	}
}

void GameLayer::SetEditor(LEditor* leveleditor, LEditorUI* editorUI)
{
	_levelEditor.reset(leveleditor);
	_levelEditorUI.reset(editorUI);
}
