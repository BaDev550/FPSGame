#pragma once
#include "Engine.h"

class GameLayer : public EngineLayer {
public:
	GameLayer();

	void OnUpdate() override;
	bool OnMouseMove(Engine::MouseMovedEvent& e);
	bool OnKeyPressed(Engine::KeyPressedEvent& e) {
		if (e.GetKeyCode() == E_KEY_L) {
			if (!_bLevelEditorOpened) {
				EngineApp::Get().QueueLayer(_levelEditor.get());
				EngineApp::Get().QueueOverlay(_levelEditorUI.get());
				_bLevelEditorOpened = true;
			}
			else {
				EngineApp::Get().QueueRemoveLayer(_levelEditor.get());
				EngineApp::Get().QueueRemoveOverlay(_levelEditorUI.get());
				_bLevelEditorOpened = false;
			}
		}

		return true;
	}

	virtual void OnImGuiRender() override {
	}

	void OnEvent(Engine::Event& event) override {
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Engine::MouseMovedEvent>(BIND_EVENT_FUNCTION(GameLayer::OnMouseMove));
		dispatcher.Dispatch<Engine::KeyPressedEvent>(BIND_EVENT_FUNCTION(GameLayer::OnKeyPressed));
	}

	Engine::CameraComponent* GetActiveCamera();
	inline std::shared_ptr<EngineScene>& GetActiveScene() { return _LoadedScene; }
	void SetEditor(EngineLayer* leveleditor, EngineLayer* editorUI);
private:
	std::shared_ptr<EngineShader> _Shader;
	std::shared_ptr<EngineShader> _ShadowShader;
	std::shared_ptr<EngineScene> _LoadedScene;
	std::shared_ptr<EngineWindow> _Window;
	std::shared_ptr<EngineRenderSystem> _RenderSystem;

	std::shared_ptr<EngineLayer> _levelEditor;
	std::shared_ptr<EngineLayer> _levelEditorUI;
	bool _bLevelEditorOpened = false;

	Entity _Camera;
private:
	float _LastX = 0.0f, _LastY = 0.0f;
	bool _bfirstPressed = true;
};