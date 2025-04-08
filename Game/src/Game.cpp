#define DEFINE_MAIN
#include <Engine.h>
#include <LevelEditor.h>
#include "GameLayer/GameLayer.h"
#include "ImGui/imgui.h"

class Game : public Engine::Application {
public:
	std::shared_ptr<GameLayer> _GameLayer;
	std::shared_ptr<Editor> _Editor;

	Game() {
		_GameLayer = std::make_shared<GameLayer>();
		_Editor = std::make_shared<Editor>(_GameLayer->GetActiveScene());

		_GameLayer->SetEditor(_Editor->GetEditor(), _Editor->GetEditorUI());
		PushLayer(_GameLayer.get());
	}
	~Game() {}
};

Engine::Application* Engine::CreateApplication() {
	return new Game();
}