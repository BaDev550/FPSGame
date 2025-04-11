#pragma once

#include "Editor/EditorLayer.h"
#include "Editor/LevelEditorLayer.h"

typedef LevelEditor::EditorLayer LEditorUI;
typedef LevelEditor::LevelEditorLayer LEditor;

class Editor {
private:
	std::shared_ptr<LEditor> _Editor;
	std::shared_ptr<LEditorUI> _EditorUI;
public:
	inline LEditorUI* GetEditorUI() { return _EditorUI.get(); }
	inline LEditor* GetEditor() { return _Editor.get(); }

	Editor() {
		_Editor = std::make_shared<LEditor>();
		_EditorUI = std::make_shared<LEditorUI>(_Editor);
	}
};