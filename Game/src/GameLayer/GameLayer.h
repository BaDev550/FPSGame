#pragma once
#include "Engine.h"
#include "LevelEditor.h"

#include "Player/PlayerPawn.h"

class GameLayer : public EngineLayer {
public:
	GameLayer();

	void OnUpdate() override;
	void OnRender();
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

		return false;
	}

	virtual void OnImGuiRender() override {
		auto screenTexture = EngineRenderer::GetScreenTexture();
		uint32_t textureID = screenTexture->GetTextureID();

		ImGui::Begin("GameViewport", nullptr);
		if (GetActiveCamera() != NULL) {
			ImVec2 _ViewportPanelSize = ImGui::GetContentRegionAvail();
			ImVec2 _ViewportPanelPos = ImGui::GetCursorScreenPos();

			ImGui::Image((void*)(intptr_t)textureID, _ViewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

			GetActiveCamera()->SetAspectRatio((float)_ViewportPanelSize.x / (float)_ViewportPanelSize.y);
		}
		ImGui::End();
	}

	void OnEvent(Engine::Event& event) override {
		EngineScene::Get().ProcessEvent(event);

		Engine::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Engine::KeyPressedEvent>(BIND_EVENT_FUNCTION(GameLayer::OnKeyPressed));
	}

	Engine::CameraComponent* GetActiveCamera();
	void SetEditor(LEditor* leveleditor, LEditorUI* editorUI);
private:
	std::shared_ptr<EngineShader> _Shader;
	std::shared_ptr<EngineWindow> _Window;
	std::shared_ptr<EngineScene> _LoadedScene;
	std::shared_ptr<EngineRenderSystem> _RenderSystem;
	std::shared_ptr<EnginePhysicsSystem> _PhysicsSystem;
	std::shared_ptr<PlayerPawn> Player;

	std::shared_ptr<LEditor> _levelEditor;
	std::shared_ptr<LEditorUI> _levelEditorUI;
	bool _bLevelEditorOpened = false;
};