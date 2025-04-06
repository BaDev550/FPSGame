#pragma once
#include <Engine.h>
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGuizmo/ImGuizmo.h"
#include "ImGui/ImGuiFileDialog.h"

class LevelEditorLayer;
class EditorLayer : public EngineLayer {
public:
	EditorLayer(std::shared_ptr<LevelEditorLayer>& layer);

	void OnUpdate() override {

	}

	void DrawEntityGizmos(entt::registry& registry, entt::entity selectedEntity);
	void DrawTopBar();
	void DrawScene(entt::registry& registry);

	virtual void OnImGuiRender() override;

	bool OnKeyPressed(Engine::KeyPressedEvent& e) {
		if (e.GetKeyCode() == E_KEY_E)
			_GizmoType = ImGuizmo::TRANSLATE;
		if (e.GetKeyCode() == E_KEY_R)
			_GizmoType = ImGuizmo::ROTATE;
		if (e.GetKeyCode() == E_KEY_Q)
			_GizmoType = ImGuizmo::SCALE;

		if (e.GetKeyCode() == E_KEY_F) {
			if (_SelectedEntity != entt::null) {
				auto& transform = _Scene->GetRegistry().get<Engine::TransformComponent>(_SelectedEntity);
				_Camera->Position = transform.Position;
			}
		}

		return false;
	}

	void OnEvent(Engine::Event& event) override {
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Engine::KeyPressedEvent>(BIND_EVENT_FUNCTION(EditorLayer::OnKeyPressed));
	}

private:
	std::shared_ptr<EngineScene>& _Scene;
	std::shared_ptr<LevelEditorLayer> _LevelEditor;
	Engine::CameraComponent* _Camera;
	Entity _SelectedEntity = entt::null;

	ImVec2 _ViewportPanelSize, _ViewportPanelPos;
	bool _ViewportHovered;
	ImGuizmo::OPERATION _GizmoType = ImGuizmo::TRANSLATE;
};