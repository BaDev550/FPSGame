#pragma once
#include <Engine.h>
#include "ImGui/imgui.h"

class EditorLayer : public EngineLayer {
public:
	EditorLayer()
		: Layer("Editor")
	{

	}

	void OnUpdate() override {

	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Start");
		ImGui::End();
	}

	void OnEvent(Engine::Event& event) override {
		Engine::EventDispatcher dispatcher(event);
	}
};