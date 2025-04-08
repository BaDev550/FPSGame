#pragma once
#include "Engine.h"

namespace LevelEditor {
	class LevelEditorLayer : public EngineLayer {
	public:
		LevelEditorLayer(std::shared_ptr<EngineScene>& scene);

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
		inline std::shared_ptr<EngineScene>& GetActiveScene() { return _LoadedScene; }

		bool _ViewportFocused = false;
	private:
		std::shared_ptr<EngineShader> _Shader;
		std::shared_ptr<EngineShader> _ShadowShader;
		std::shared_ptr<EngineScene> _LoadedScene;
		std::shared_ptr<EngineWindow> _Window;
		std::shared_ptr<EngineRenderSystem> _RenderSystem;

		Entity _Camera;
	private:
		float _LastX = 0.0f, _LastY = 0.0f;
		bool _bfirstPressed = true;
		bool _bCursor = false;
	};
}