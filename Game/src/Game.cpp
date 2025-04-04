#include <Engine.h>
#include "imgui/imgui.h"

class Game : public EngineLayer {
public:
	Game()
		: Layer("GameLayer")
	{
		float aspectRatio = 1280 / 720;
		_Camera.reset(new EngineCamera({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, -90.0f, 0.0f, aspectRatio));
		_Mario = EngineLoadStaticMesh("Assets/Models/mario_2/mario_2.obj");
		_Shader = EngineCreateShader("Assets/Shaders/base_vertex.glsl", "Assets/Shaders/base_fragment.glsl");
	}

	void OnUpdate() override {
		if (EngineKeyPressed(E_KEY_W))
			_Camera->ProcessKeyboard(Engine::ECameraDirection::FORWARD, 0.1f);
		if (EngineKeyPressed(E_KEY_S))
			_Camera->ProcessKeyboard(Engine::ECameraDirection::BACKWARD, 0.1f);
		if (EngineKeyPressed(E_KEY_A))
			_Camera->ProcessKeyboard(Engine::ECameraDirection::LEFT, 0.1f);
		if (EngineKeyPressed(E_KEY_D))
			_Camera->ProcessKeyboard(Engine::ECameraDirection::RIGHT, 0.1f);

		EngineSetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		EngineClear();

		RendererBeginScene(*_Camera);

		_Mario->Draw(_Shader);

		RendererEndScene();
	}

	float lastX = 0.0f, lastY = 0.0f;
	bool OnMouseMove(Engine::MouseMovedEvent& e)
	{
		float xOffset = e.GetX() - lastX;
		float yOffset = lastY - e.GetY();
		lastX = e.GetX();
		lastY = e.GetY();

		_Camera->ProcessMouseMovement(xOffset, yOffset);
		return true;
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::Text("Mouse Position: (%.1f, %.1f)", lastX, lastY);
		ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)", _Camera->GetPosition().x, _Camera->GetPosition().y, _Camera->GetPosition().z);
		ImGui::End();
	}

	void OnEvent(Engine::Event& event) override {
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Engine::MouseMovedEvent>(BIND_EVENT_FUNCTION(Game::OnMouseMove));
	}

private:
	std::shared_ptr<EngineShader> _Shader;
	std::shared_ptr<EngineCamera> _Camera;
	std::unique_ptr<EngineModel> _Mario;
};

class Sandbox : public Engine::Application {
public:
	Sandbox() {
		PushLayer(new Game());
	}
	~Sandbox() {}
};

Engine::Application* Engine::CreateApplication() {
	return new Sandbox();
}