#define DEFINE_MAIN
#include <Engine.h>
#include "Editor.h"
#include <memory>

class LevelEditorLayer : public EngineLayer {
public:
	LevelEditorLayer()
		: Layer("LevelEditor")
	{
		_Window = std::make_unique<EngineWindow>(EngineApp::Get().GetWindow());
		float aspectRatio = (float)_Window->GetWidth() / (float)_Window->GetHeight();

		_Shader = EngineCreateShader("../Game/Assets/Shaders/base_vertex.glsl", "../Game/Assets/Shaders/base_fragment.glsl");
		_LoadedScene = std::make_shared<EngineScene>();
		_EditorCamera = std::make_shared<EngineCamera>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, aspectRatio);
		_RenderSystem = std::make_unique<EngineRenderSystem>(_Shader);

		_Mario = _LoadedScene->CreateEntity("Mario");
		_LoadedScene->GetRegistry().emplace<EngineStaticMeshComponent>(_Mario, "../Game/Assets/Models/mario_2/mario_2.obj");
	}

	void OnUpdate() override {
		if (EngineKeyPressed(E_KEY_W))
			_EditorCamera->ProcessKeyboard(Engine::ECameraDirection::FORWARD, 0.1f);
		if (EngineKeyPressed(E_KEY_S))
			_EditorCamera->ProcessKeyboard(Engine::ECameraDirection::BACKWARD, 0.1f);
		if (EngineKeyPressed(E_KEY_A))
			_EditorCamera->ProcessKeyboard(Engine::ECameraDirection::LEFT, 0.1f);
		if (EngineKeyPressed(E_KEY_D))
			_EditorCamera->ProcessKeyboard(Engine::ECameraDirection::RIGHT, 0.1f);

		EngineSetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		EngineClear();

		RendererBeginScene(*_EditorCamera);

		_RenderSystem->Draw(_LoadedScene->GetRegistry());

		RendererEndScene();
	}

	bool OnMouseMove(Engine::MouseMovedEvent& e)
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

		_EditorCamera->ProcessMouseMovement(xOffset, yOffset);
		return true;
	}

	bool OnKeyPressed(Engine::KeyPressedEvent& e) {
		if (e.GetKeyCode() == E_KEY_TAB) {
			_bCursor = !_bCursor;
			_bfirstPressed = true;
			_Window->SetInputMode(GLFW_CURSOR, _bCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		}

		return true;
	}

	virtual void OnImGuiRender() override {

	}

	void OnEvent(Engine::Event& event) override {
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Engine::MouseMovedEvent>(BIND_EVENT_FUNCTION(LevelEditorLayer::OnMouseMove));
		dispatcher.Dispatch<Engine::KeyPressedEvent>(BIND_EVENT_FUNCTION(LevelEditorLayer::OnKeyPressed));
	}
private:
	std::shared_ptr<EngineShader> _Shader;
	std::shared_ptr<EngineCamera> _EditorCamera;
	std::shared_ptr<EngineScene> _LoadedScene;
	std::unique_ptr<EngineWindow> _Window;
	std::unique_ptr<EngineRenderSystem> _RenderSystem;

	Entity _Mario;
private:
	float _LastX = 0.0f, _LastY = 0.0f;
	bool _bfirstPressed = true;
	bool _bCursor = false;
};

class LevelEditor : public Engine::Application {
public:
	LevelEditor() {
		PushLayer(new LevelEditorLayer());
		PushLayer(new EditorLayer());
	}
	~LevelEditor() {}
};

Engine::Application* Engine::CreateApplication() {
	return new LevelEditor();
}