#pragma once
#include "Engine.h"

namespace LevelEditor {
	class LevelEditorLayer : public EngineLayer {
	public:
		LevelEditorLayer();

		void OnUpdate() override;
		bool OnMouseMove(Engine::MouseMovedEvent& e);

		bool OnKeyPressed(Engine::KeyPressedEvent& e) {
			if (e.GetKeyCode() == E_KEY_TAB) {
				_bCursor = !_bCursor;
				_bfirstPressed = true;
				_Window->SetInputMode(GLFW_CURSOR, _bCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
			}

			return false;
		}

		virtual void OnImGuiRender() override {

		}

		void OnEvent(Engine::Event& event) override {
			Engine::EventDispatcher dispatcher(event);
			dispatcher.Dispatch<Engine::MouseMovedEvent>(BIND_EVENT_FUNCTION(LevelEditorLayer::OnMouseMove));
			dispatcher.Dispatch<Engine::KeyPressedEvent>(BIND_EVENT_FUNCTION(LevelEditorLayer::OnKeyPressed));
		}

		Engine::CameraComponent* GetActiveCamera();

		bool _ViewportFocused = false;
		EngineEntity _EditorCamera;
	private:
		std::shared_ptr<EngineWindow> _Window;

	private:
		float _LastX = 0.0f, _LastY = 0.0f;
		bool _bfirstPressed = true;
		bool _bCursor = false;
	};
}