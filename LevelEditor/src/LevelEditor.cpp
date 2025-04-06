#define DEFINE_MAIN
#include <Engine.h>
#include "EditorLayer.h"
#include "LevelEditorLayer.h"
#include <memory>

class LevelEditor : public Engine::Application {
public:
	std::shared_ptr<LevelEditorLayer> _EditorLayer = std::make_shared<LevelEditorLayer>();
	std::shared_ptr<EditorLayer> _Editor = std::make_shared<EditorLayer>(_EditorLayer);

	LevelEditor() {
		PushLayer(_EditorLayer.get());
		PushOverlay(_Editor.get());
	}
	~LevelEditor() {}
};

Engine::Application* Engine::CreateApplication() {
	return new LevelEditor();
}