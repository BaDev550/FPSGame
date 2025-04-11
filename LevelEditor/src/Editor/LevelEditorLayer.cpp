#include "LevelEditorLayer.h"

namespace LevelEditor {
	LevelEditorLayer::LevelEditorLayer() : Layer("LevelEditor")
	{
		_Window = std::make_shared<EngineWindow>(EngineApp::Get().GetWindow());
		_EditorCamera = EngineScene::Get().CreateCamera("EditorCamera");
	}

	void LevelEditorLayer::OnUpdate()
	{
		if (EngineKeyPressed(E_KEY_W) && _ViewportFocused)
			GetActiveCamera()->ProcessKeyboard(Engine::ECameraDirection::FORWARD, 0.1f);
		if (EngineKeyPressed(E_KEY_S) && _ViewportFocused)
			GetActiveCamera()->ProcessKeyboard(Engine::ECameraDirection::BACKWARD, 0.1f);
		if (EngineKeyPressed(E_KEY_A) && _ViewportFocused)
			GetActiveCamera()->ProcessKeyboard(Engine::ECameraDirection::LEFT, 0.1f);
		if (EngineKeyPressed(E_KEY_D) && _ViewportFocused)
			GetActiveCamera()->ProcessKeyboard(Engine::ECameraDirection::RIGHT, 0.1f);
	}

	bool LevelEditorLayer::OnMouseMove(Engine::MouseMovedEvent& e)
	{
		if (_bCursor) return false;

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
		return true;
	}

	Engine::CameraComponent* LevelEditorLayer::GetActiveCamera()
	{
		if (_EditorCamera.IsValid() && _EditorCamera.HasComponent<Engine::CameraComponent>()) {
			auto& camera = _EditorCamera.GetComponent<Engine::CameraComponent>();
			return &camera;
		}
		else {
			std::cout << "No Active Camera" << std::endl;
		}
	}
}