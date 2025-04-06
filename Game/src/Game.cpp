#define DEFINE_MAIN
#include <Engine.h>

class Game : public EngineLayer {
public:
	Game()
		: Layer("GameLayer")
	{
		EngineWindow* window = &EngineApp::Get().GetWindow();
		float aspectRatio = (float)window->GetWidth() / (float)window->GetHeight();
		//_Camera.reset(new EngineCamera({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, -90.0f, 0.0f, aspectRatio));
		_Shader = EngineCreateShader("Assets/Shaders/base_vertex.glsl", "Assets/Shaders/base_fragment.glsl");
	}

	void OnUpdate() override {
		EngineSetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		EngineClear();

		//RendererBeginScene(*_Camera);

		RendererEndScene();
	}

	float lastX = 0.0f, lastY = 0.0f;
	bool bfirstPressed = true;
	bool OnMouseMove(Engine::MouseMovedEvent& e)
	{
		if (bfirstPressed)
		{
			lastX = e.GetX();
			lastY = e.GetY();
			bfirstPressed = false;
			return false;
		}

		float xOffset = e.GetX() - lastX;
		float yOffset = lastY - e.GetY();

		lastX = e.GetX();
		lastY = e.GetY();

		//_Camera->ProcessMouseMovement(xOffset, yOffset);
		return true;
	}

	virtual void OnImGuiRender() override {

	}

	void OnEvent(Engine::Event& event) override {
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Engine::MouseMovedEvent>(BIND_EVENT_FUNCTION(Game::OnMouseMove));
	}

private:
	std::shared_ptr<EngineShader> _Shader;
	std::shared_ptr<EngineScene> _Scene;
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